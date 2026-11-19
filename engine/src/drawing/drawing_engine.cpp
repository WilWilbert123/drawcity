#include "living_canvas/drawing/drawing_engine.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>

namespace living_canvas::drawing {

DrawingEngine::DrawingEngine() {
    create_layer("Base Artwork Layer");
}

int64_t DrawingEngine::create_layer(const std::string& name) {
    const int64_t id = next_layer_id_++;
    const int32_t z = static_cast<int32_t>(layers_.size());
    layers_.push_back(LayerData{id, name.empty() ? ("Layer " + std::to_string(id)) : name, z, 1.0f, true, false});
    return id;
}

bool DrawingEngine::remove_layer(int64_t layer_id) {
    if (layers_.size() <= 1) {
        return false;
    }
    auto it = std::find_if(layers_.begin(), layers_.end(),
                           [layer_id](const LayerData& l) { return l.layer_id == layer_id; });
    if (it == layers_.end()) {
        return false;
    }
    layers_.erase(it);
    strokes_.erase(std::remove_if(strokes_.begin(), strokes_.end(),
                                  [layer_id](const Stroke& s) { return s.layer_id() == layer_id; }),
                   strokes_.end());
    return true;
}

bool DrawingEngine::set_layer_visibility(int64_t layer_id, bool visible) {
    for (auto& layer : layers_) {
        if (layer.layer_id == layer_id) {
            layer.visible = visible;
            return true;
        }
    }
    return false;
}

bool DrawingEngine::set_layer_locked(int64_t layer_id, bool locked) {
    for (auto& layer : layers_) {
        if (layer.layer_id == layer_id) {
            layer.locked = locked;
            return true;
        }
    }
    return false;
}

int64_t DrawingEngine::begin_stroke(int64_t layer_id, float x, float y, float pressure) {
    for (const auto& layer : layers_) {
        if (layer.layer_id == layer_id && layer.locked) {
            return 0;
        }
    }
    const int64_t id = next_stroke_id_++;
    const auto& brush = brush_engine_.current_brush();
    Stroke stroke(id, layer_id, brush.type, brush_engine_.color_hex(), brush.default_size);
    stroke.set_stroke_order(static_cast<int32_t>(strokes_.size()));
    stroke.add_point(x, y, pressure);
    strokes_.push_back(std::move(stroke));
    return id;
}

void DrawingEngine::append_stroke_point(int64_t stroke_id, float x, float y, float pressure) {
    auto* stroke = find_mutable_stroke(stroke_id);
    if (!stroke) {
        return;
    }
    stroke->add_point(x, y, pressure);
}

bool DrawingEngine::end_stroke(int64_t stroke_id) {
    auto* stroke = find_mutable_stroke(stroke_id);
    if (!stroke) {
        return false;
    }
    stroke->apply_smoothing(brush_engine_.current_brush().smoothing);
    return true;
}

int32_t DrawingEngine::erase_near(int64_t layer_id, float x, float y, float radius) {
    const float r2 = radius * radius;
    int32_t removed = 0;
    auto it = strokes_.begin();
    while (it != strokes_.end()) {
        if (it->layer_id() != layer_id || it->is_locked()) {
            ++it;
            continue;
        }
        bool hit = false;
        for (const auto& pt : it->raw_points()) {
            const float dx = pt.x - x;
            const float dy = pt.y - y;
            if ((dx * dx + dy * dy) <= r2) {
                hit = true;
                break;
            }
        }
        if (hit) {
            it = strokes_.erase(it);
            ++removed;
        } else {
            ++it;
        }
    }
    return removed;
}

void DrawingEngine::clear_canvas() {
    strokes_.clear();
}

std::vector<int64_t> DrawingEngine::select_strokes_in_rect(float min_x, float min_y, float max_x, float max_y) const {
    const float left = std::min(min_x, max_x);
    const float right = std::max(min_x, max_x);
    const float top = std::min(min_y, max_y);
    const float bottom = std::max(min_y, max_y);

    std::vector<int64_t> selected;
    for (const auto& stroke : strokes_) {
        if (!stroke.is_visible()) {
            continue;
        }
        for (const auto& pt : stroke.raw_points()) {
            if (pt.x >= left && pt.x <= right && pt.y >= top && pt.y <= bottom) {
                selected.push_back(stroke.id());
                break;
            }
        }
    }
    return selected;
}

void DrawingEngine::translate_strokes(const std::vector<int64_t>& stroke_ids, float dx, float dy) {
    for (int64_t id : stroke_ids) {
        if (auto* s = find_mutable_stroke(id)) {
            s->translate(dx, dy);
        }
    }
}

void DrawingEngine::scale_strokes(const std::vector<int64_t>& stroke_ids, float sx, float sy, float pivot_x, float pivot_y) {
    for (int64_t id : stroke_ids) {
        if (auto* s = find_mutable_stroke(id)) {
            s->scale(sx, sy, pivot_x, pivot_y);
        }
    }
}

void DrawingEngine::rotate_strokes(const std::vector<int64_t>& stroke_ids, float angle_radians, float pivot_x, float pivot_y) {
    for (int64_t id : stroke_ids) {
        if (auto* s = find_mutable_stroke(id)) {
            s->rotate(angle_radians, pivot_x, pivot_y);
        }
    }
}

void DrawingEngine::assign_strokes_to_object(const std::vector<int64_t>& stroke_ids, int64_t object_id, int64_t part_id) {
    for (int64_t id : stroke_ids) {
        if (auto* s = find_mutable_stroke(id)) {
            s->set_object_id(object_id);
            if (part_id > 0) {
                s->set_part_id(part_id);
            }
        }
    }
}

const Stroke* DrawingEngine::find_stroke(int64_t stroke_id) const {
    for (const auto& s : strokes_) {
        if (s.id() == stroke_id) {
            return &s;
        }
    }
    return nullptr;
}

Stroke* DrawingEngine::find_mutable_stroke(int64_t stroke_id) {
    for (auto& s : strokes_) {
        if (s.id() == stroke_id) {
            return &s;
        }
    }
    return nullptr;
}

void DrawingEngine::set_strokes(std::vector<Stroke> strokes) {
    strokes_ = std::move(strokes);
    int64_t max_id = 0;
    for (const auto& s : strokes_) {
        max_id = std::max(max_id, s.id());
    }
    next_stroke_id_ = max_id + 1;
}

std::string DrawingEngine::serialize_canvas_json() const {
    std::ostringstream out;
    out << "{\"layers\":[";
    for (std::size_t i = 0; i < layers_.size(); ++i) {
        if (i > 0) {
            out << ",";
        }
        out << "{"
            << "\"id\":" << layers_[i].layer_id << ","
            << "\"name\":\"" << layers_[i].name << "\","
            << "\"z_index\":" << layers_[i].z_index << ","
            << "\"opacity\":" << layers_[i].opacity << ","
            << "\"visible\":" << (layers_[i].visible ? "true" : "false") << ","
            << "\"locked\":" << (layers_[i].locked ? "true" : "false")
            << "}";
    }
    out << "],\"strokes\":[";
    for (std::size_t i = 0; i < strokes_.size(); ++i) {
        if (i > 0) {
            out << ",";
        }
        out << strokes_[i].to_json();
    }
    out << "]}";
    return out.str();
}

} // namespace living_canvas::drawing

