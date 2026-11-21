#include "drawing/drawing_bridge.hpp"

#include <cmath>
#include <godot_cpp/core/class_db.hpp>

namespace living_canvas::gdext {

DrawingBridge::DrawingBridge() {
    history_.push_snapshot("initial", engine_.serialize_canvas_json());
}

int64_t DrawingBridge::create_layer(const godot::String& name) {
    return engine_.create_layer(name.utf8().get_data());
}

bool DrawingBridge::set_layer_visibility(int64_t layer_id, bool visible) {
    return engine_.set_layer_visibility(layer_id, visible);
}

bool DrawingBridge::set_layer_locked(int64_t layer_id, bool locked) {
    return engine_.set_layer_locked(layer_id, locked);
}

int32_t DrawingBridge::get_layer_count() const {
    return static_cast<int32_t>(engine_.layers().size());
}

void DrawingBridge::select_brush(int32_t brush_type) {
    engine_.brush_engine().select_brush(static_cast<living_canvas::drawing::BrushType>(brush_type));
}

void DrawingBridge::set_brush_size(double size) {
    engine_.brush_engine().set_brush_size(static_cast<float>(size));
}

double DrawingBridge::get_brush_size() const {
    return engine_.brush_engine().current_brush().default_size;
}

void DrawingBridge::set_brush_color(const godot::String& color_hex) {
    engine_.brush_engine().set_color_hex(color_hex.utf8().get_data());
}

godot::String DrawingBridge::get_brush_color() const {
    return godot::String(engine_.brush_engine().color_hex().c_str());
}

int64_t DrawingBridge::begin_stroke(int64_t layer_id, double x, double y, double pressure) {
    return engine_.begin_stroke(layer_id, static_cast<float>(x), static_cast<float>(y), static_cast<float>(pressure));
}

void DrawingBridge::append_stroke_point(int64_t stroke_id, double x, double y, double pressure) {
    engine_.append_stroke_point(stroke_id, static_cast<float>(x), static_cast<float>(y), static_cast<float>(pressure));
}

bool DrawingBridge::end_stroke(int64_t stroke_id) {
    const bool ok = engine_.end_stroke(stroke_id);
    if (ok) {
        history_.push_snapshot("stroke", engine_.serialize_canvas_json());
    }
    return ok;
}

int32_t DrawingBridge::erase_near(int64_t layer_id, double x, double y, double radius) {
    const int32_t erased = engine_.erase_near(layer_id, static_cast<float>(x), static_cast<float>(y), static_cast<float>(radius));
    if (erased > 0) {
        history_.push_snapshot("erase", engine_.serialize_canvas_json());
    }
    return erased;
}

void DrawingBridge::clear_canvas() {
    engine_.clear_canvas();
    history_.push_snapshot("clear", engine_.serialize_canvas_json());
}

int32_t DrawingBridge::get_stroke_count() const {
    return static_cast<int32_t>(engine_.strokes().size());
}

godot::PackedVector2Array DrawingBridge::get_stroke_points(int64_t stroke_id) const {
    godot::PackedVector2Array arr;
    const auto* s = engine_.find_stroke(stroke_id);
    if (!s) {
        return arr;
    }
    const auto& pts = s->smoothed_points();
    arr.resize(static_cast<int64_t>(pts.size()));
    for (std::size_t i = 0; i < pts.size(); ++i) {
        arr.set(static_cast<int64_t>(i), godot::Vector2(pts[i].x, pts[i].y));
    }
    return arr;
}

godot::String DrawingBridge::get_stroke_color(int64_t stroke_id) const {
    const auto* s = engine_.find_stroke(stroke_id);
    return s ? godot::String(s->color_hex().c_str()) : godot::String("#F9FAFB");
}

double DrawingBridge::get_stroke_width(int64_t stroke_id) const {
    const auto* s = engine_.find_stroke(stroke_id);
    return s ? s->base_width() : 4.0;
}

godot::PackedInt64Array DrawingBridge::select_strokes_in_rect(double min_x, double min_y, double max_x, double max_y) const {
    const auto vec = engine_.select_strokes_in_rect(static_cast<float>(min_x), static_cast<float>(min_y),
                                                    static_cast<float>(max_x), static_cast<float>(max_y));
    godot::PackedInt64Array arr;
    arr.resize(static_cast<int64_t>(vec.size()));
    for (std::size_t i = 0; i < vec.size(); ++i) {
        arr.set(static_cast<int64_t>(i), vec[i]);
    }
    return arr;
}

void DrawingBridge::translate_strokes(const godot::PackedInt64Array& stroke_ids, double dx, double dy) {
    std::vector<int64_t> ids(static_cast<std::size_t>(stroke_ids.size()));
    for (int64_t i = 0; i < stroke_ids.size(); ++i) {
        ids[static_cast<std::size_t>(i)] = stroke_ids[i];
    }
    engine_.translate_strokes(ids, static_cast<float>(dx), static_cast<float>(dy));
    history_.push_snapshot("translate", engine_.serialize_canvas_json());
}

void DrawingBridge::scale_strokes(const godot::PackedInt64Array& stroke_ids, double sx, double sy, double pivot_x, double pivot_y) {
    std::vector<int64_t> ids(static_cast<std::size_t>(stroke_ids.size()));
    for (int64_t i = 0; i < stroke_ids.size(); ++i) {
        ids[static_cast<std::size_t>(i)] = stroke_ids[i];
    }
    engine_.scale_strokes(ids, static_cast<float>(sx), static_cast<float>(sy), static_cast<float>(pivot_x), static_cast<float>(pivot_y));
    history_.push_snapshot("scale", engine_.serialize_canvas_json());
}

void DrawingBridge::rotate_strokes(const godot::PackedInt64Array& stroke_ids, double angle_rad, double pivot_x, double pivot_y) {
    std::vector<int64_t> ids(static_cast<std::size_t>(stroke_ids.size()));
    for (int64_t i = 0; i < stroke_ids.size(); ++i) {
        ids[static_cast<std::size_t>(i)] = stroke_ids[i];
    }
    engine_.rotate_strokes(ids, static_cast<float>(angle_rad), static_cast<float>(pivot_x), static_cast<float>(pivot_y));
    history_.push_snapshot("rotate", engine_.serialize_canvas_json());
}

bool DrawingBridge::can_undo() const {
    return history_.can_undo();
}

bool DrawingBridge::can_redo() const {
    return history_.can_redo();
}

godot::String DrawingBridge::undo() {
    const auto snap = history_.undo();
    return godot::String(snap.state_json.c_str());
}

godot::String DrawingBridge::redo() {
    const auto snap = history_.redo();
    return godot::String(snap.state_json.c_str());
}

void DrawingBridge::load_sample_stickman() {
    engine_.brush_engine().select_brush(living_canvas::drawing::BrushType::Pen);
    engine_.brush_engine().set_color_hex("#F59E0B");
    engine_.brush_engine().set_brush_size(5.0f);

    // Head circle around (470, 200), radius 28
    int64_t s_head = engine_.begin_stroke(1, 470.0f + 28.0f, 200.0f, 1.0f);
    for (int i = 1; i <= 16; ++i) {
        float ang = static_cast<float>(i) * (6.2831853f / 16.0f);
        engine_.append_stroke_point(s_head, 470.0f + std::cos(ang) * 28.0f, 200.0f + std::sin(ang) * 28.0f, 1.0f);
    }
    engine_.end_stroke(s_head);

    // Torso
    int64_t s_body = engine_.begin_stroke(1, 470.0f, 228.0f, 1.0f);
    engine_.append_stroke_point(s_body, 470.0f, 340.0f, 1.0f);
    engine_.end_stroke(s_body);

    // Left Arm
    int64_t s_larm = engine_.begin_stroke(1, 470.0f, 250.0f, 1.0f);
    engine_.append_stroke_point(s_larm, 425.0f, 285.0f, 1.0f);
    engine_.append_stroke_point(s_larm, 395.0f, 315.0f, 1.0f);
    engine_.end_stroke(s_larm);

    // Right Arm
    int64_t s_rarm = engine_.begin_stroke(1, 470.0f, 250.0f, 1.0f);
    engine_.append_stroke_point(s_rarm, 515.0f, 285.0f, 1.0f);
    engine_.append_stroke_point(s_rarm, 545.0f, 315.0f, 1.0f);
    engine_.end_stroke(s_rarm);

    // Left Leg
    int64_t s_lleg = engine_.begin_stroke(1, 470.0f, 340.0f, 1.0f);
    engine_.append_stroke_point(s_lleg, 445.0f, 410.0f, 1.0f);
    engine_.append_stroke_point(s_lleg, 430.0f, 480.0f, 1.0f);
    engine_.end_stroke(s_lleg);

    // Right Leg
    int64_t s_rleg = engine_.begin_stroke(1, 470.0f, 340.0f, 1.0f);
    engine_.append_stroke_point(s_rleg, 495.0f, 410.0f, 1.0f);
    engine_.append_stroke_point(s_rleg, 510.0f, 480.0f, 1.0f);
    engine_.end_stroke(s_rleg);

    history_.push_snapshot("sample_stickman", engine_.serialize_canvas_json());
}

void DrawingBridge::load_sample_car() {
    engine_.brush_engine().select_brush(living_canvas::drawing::BrushType::Pen);
    engine_.brush_engine().set_color_hex("#3B82F6");
    engine_.brush_engine().set_brush_size(5.5f);

    // Car chassis
    int64_t s_body = engine_.begin_stroke(1, 680.0f, 440.0f, 1.0f);
    engine_.append_stroke_point(s_body, 710.0f, 390.0f, 1.0f);
    engine_.append_stroke_point(s_body, 830.0f, 390.0f, 1.0f);
    engine_.append_stroke_point(s_body, 880.0f, 440.0f, 1.0f);
    engine_.append_stroke_point(s_body, 920.0f, 445.0f, 1.0f);
    engine_.append_stroke_point(s_body, 920.0f, 475.0f, 1.0f);
    engine_.append_stroke_point(s_body, 660.0f, 475.0f, 1.0f);
    engine_.append_stroke_point(s_body, 660.0f, 440.0f, 1.0f);
    engine_.append_stroke_point(s_body, 680.0f, 440.0f, 1.0f);
    engine_.end_stroke(s_body);

    // Left & Right wheels
    engine_.brush_engine().set_color_hex("#10B981");
    for (float wx : {715.0f, 865.0f}) {
        int64_t sw = engine_.begin_stroke(1, wx + 26.0f, 480.0f, 1.0f);
        for (int i = 1; i <= 14; ++i) {
            float ang = static_cast<float>(i) * (6.2831853f / 14.0f);
            engine_.append_stroke_point(sw, wx + std::cos(ang) * 26.0f, 480.0f + std::sin(ang) * 26.0f, 1.0f);
        }
        engine_.end_stroke(sw);
    }

    history_.push_snapshot("sample_car", engine_.serialize_canvas_json());
}

void DrawingBridge::load_sample_fish_and_ocean() {
    engine_.brush_engine().select_brush(living_canvas::drawing::BrushType::Marker);
    engine_.brush_engine().set_color_hex("#06B6D4");
    engine_.brush_engine().set_brush_size(8.0f);

    // Ocean wave
    int64_t s_wave = engine_.begin_stroke(1, 100.0f, 540.0f, 1.0f);
    for (int i = 1; i <= 24; ++i) {
        float x = 100.0f + static_cast<float>(i) * 45.0f;
        float y = 540.0f + std::sin(static_cast<float>(i) * 0.8f) * 14.0f;
        engine_.append_stroke_point(s_wave, x, y, 1.0f);
    }
    engine_.end_stroke(s_wave);

    // Drawn Fish body + tail
    engine_.brush_engine().select_brush(living_canvas::drawing::BrushType::Pen);
    engine_.brush_engine().set_color_hex("#EC4899");
    engine_.brush_engine().set_brush_size(4.5f);
    int64_t s_fish = engine_.begin_stroke(1, 320.0f, 590.0f, 1.0f);
    engine_.append_stroke_point(s_fish, 360.0f, 570.0f, 1.0f);
    engine_.append_stroke_point(s_fish, 410.0f, 590.0f, 1.0f);
    engine_.append_stroke_point(s_fish, 440.0f, 570.0f, 1.0f);
    engine_.append_stroke_point(s_fish, 440.0f, 610.0f, 1.0f);
    engine_.append_stroke_point(s_fish, 410.0f, 590.0f, 1.0f);
    engine_.append_stroke_point(s_fish, 360.0f, 610.0f, 1.0f);
    engine_.append_stroke_point(s_fish, 320.0f, 590.0f, 1.0f);
    engine_.end_stroke(s_fish);

    history_.push_snapshot("sample_fish_ocean", engine_.serialize_canvas_json());
}

void DrawingBridge::load_sample_bird_and_sky() {
    engine_.brush_engine().select_brush(living_canvas::drawing::BrushType::Pen);
    engine_.brush_engine().set_color_hex("#A855F7");
    engine_.brush_engine().set_brush_size(4.5f);

    // Drawn Bird wings + body
    int64_t s_bird = engine_.begin_stroke(1, 240.0f, 130.0f, 1.0f);
    engine_.append_stroke_point(s_bird, 275.0f, 105.0f, 1.0f);
    engine_.append_stroke_point(s_bird, 300.0f, 130.0f, 1.0f);
    engine_.append_stroke_point(s_bird, 325.0f, 105.0f, 1.0f);
    engine_.append_stroke_point(s_bird, 360.0f, 130.0f, 1.0f);
    engine_.end_stroke(s_bird);

    history_.push_snapshot("sample_bird", engine_.serialize_canvas_json());
}

void DrawingBridge::load_sample_city() {
    // 1. Two City Buildings + Windows (part_id = 10 -> static building)
    engine_.brush_engine().select_brush(living_canvas::drawing::BrushType::Pen);
    engine_.brush_engine().set_color_hex("#64748B");
    engine_.brush_engine().set_brush_size(5.0f);

    int64_t b1 = engine_.begin_stroke(1, 110.0f, 620.0f, 1.0f);
    engine_.append_stroke_point(b1, 110.0f, 240.0f, 1.0f);
    engine_.append_stroke_point(b1, 290.0f, 240.0f, 1.0f);
    engine_.append_stroke_point(b1, 290.0f, 620.0f, 1.0f);
    engine_.end_stroke(b1);

    int64_t b2 = engine_.begin_stroke(1, 1020.0f, 620.0f, 1.0f);
    engine_.append_stroke_point(b2, 1020.0f, 180.0f, 1.0f);
    engine_.append_stroke_point(b2, 1240.0f, 180.0f, 1.0f);
    engine_.append_stroke_point(b2, 1240.0f, 620.0f, 1.0f);
    engine_.end_stroke(b2);

    //Lit Windows
    engine_.brush_engine().set_color_hex("#FACC15");
    engine_.brush_engine().set_brush_size(4.0f);
    for (float wy : {290.0f, 360.0f, 430.0f, 500.0f}) {
        for (float wx : {150.0f, 220.0f, 1070.0f, 1160.0f}) {
            int64_t sw = engine_.begin_stroke(1, wx, wy, 1.0f);
            engine_.append_stroke_point(sw, wx + 28.0f, wy, 1.0f);
            engine_.append_stroke_point(sw, wx + 28.0f, wy + 28.0f, 1.0f);
            engine_.append_stroke_point(sw, wx, wy + 28.0f, 1.0f);
            engine_.append_stroke_point(sw, wx, wy, 1.0f);
            engine_.end_stroke(sw);
        }
    }
    commit_unassigned_strokes_to_object(101, 10);

    // 2. City Tree (part_id = 11 -> static tree)
    engine_.brush_engine().set_color_hex("#A16207");
    engine_.brush_engine().set_brush_size(6.0f);
    int64_t tr_trunk = engine_.begin_stroke(1, 350.0f, 620.0f, 1.0f);
    engine_.append_stroke_point(tr_trunk, 350.0f, 510.0f, 1.0f);
    engine_.end_stroke(tr_trunk);

    engine_.brush_engine().set_color_hex("#10B981");
    int64_t tr_crown = engine_.begin_stroke(1, 350.0f + 42.0f, 480.0f, 1.0f);
    for (int i = 1; i <= 14; ++i) {
        float ang = static_cast<float>(i) * (6.2831853f / 14.0f);
        engine_.append_stroke_point(tr_crown, 350.0f + std::cos(ang) * 42.0f, 480.0f + std::sin(ang) * 38.0f, 1.0f);
    }
    engine_.end_stroke(tr_crown);
    commit_unassigned_strokes_to_object(102, 11);

    // 3. Drifting Sky Clouds (part_id = 3 -> drifting cloud)
    engine_.brush_engine().set_color_hex("#E2E8F0");
    engine_.brush_engine().set_brush_size(5.0f);
    for (float cx : {220.0f, 740.0f}) {
        int64_t sc = engine_.begin_stroke(1, cx, 110.0f, 1.0f);
        engine_.append_stroke_point(sc, cx + 35.0f, 90.0f, 1.0f);
        engine_.append_stroke_point(sc, cx + 75.0f, 95.0f, 1.0f);
        engine_.append_stroke_point(sc, cx + 110.0f, 110.0f, 1.0f);
        engine_.append_stroke_point(sc, cx, 110.0f, 1.0f);
        engine_.end_stroke(sc);
    }
    commit_unassigned_strokes_to_object(103, 3);

    // 4. Flying Bird (part_id = 4 -> flying bird)
    load_sample_bird_and_sky();
    commit_unassigned_strokes_to_object(104, 4);

    // 5. Drawn Car (object_id = 2, part_id = 2 -> drivable car)
    load_sample_car();
    commit_unassigned_strokes_to_object(2, 2);

    // 6. Drawn Stickman Citizen (object_id = 1, part_id = 1 -> rigged stickman citizen)
    load_sample_stickman();
    commit_unassigned_strokes_to_object(1, 1);

    history_.push_snapshot("sample_city", engine_.serialize_canvas_json());
}

int32_t DrawingBridge::commit_unassigned_strokes_to_object(int64_t object_id, int64_t part_id) {
    int32_t count = 0;
    std::vector<int64_t> ids;
    for (const auto& s : engine_.strokes()) {
        if (s.object_id() == 0) {
            ids.push_back(s.id());
            ++count;
        }
    }
    if (!ids.empty()) {
        engine_.assign_strokes_to_object(ids, object_id, part_id);
        history_.push_snapshot("commit_city_object", engine_.serialize_canvas_json());
    }
    return count;
}

int32_t DrawingBridge::get_unassigned_stroke_count() const {
    int32_t count = 0;
    for (const auto& s : engine_.strokes()) {
        if (s.object_id() == 0) {
            ++count;
        }
    }
    return count;
}

godot::String DrawingBridge::serialize_canvas_json() const {
    return godot::String(engine_.serialize_canvas_json().c_str());
}

void DrawingBridge::_bind_methods() {
    using namespace godot;
    ClassDB::bind_method(D_METHOD("create_layer", "name"), &DrawingBridge::create_layer);
    ClassDB::bind_method(D_METHOD("set_layer_visibility", "layer_id", "visible"), &DrawingBridge::set_layer_visibility);
    ClassDB::bind_method(D_METHOD("set_layer_locked", "layer_id", "locked"), &DrawingBridge::set_layer_locked);
    ClassDB::bind_method(D_METHOD("get_layer_count"), &DrawingBridge::get_layer_count);

    ClassDB::bind_method(D_METHOD("select_brush", "brush_type"), &DrawingBridge::select_brush);
    ClassDB::bind_method(D_METHOD("set_brush_size", "size"), &DrawingBridge::set_brush_size);
    ClassDB::bind_method(D_METHOD("get_brush_size"), &DrawingBridge::get_brush_size);
    ClassDB::bind_method(D_METHOD("set_brush_color", "color_hex"), &DrawingBridge::set_brush_color);
    ClassDB::bind_method(D_METHOD("get_brush_color"), &DrawingBridge::get_brush_color);

    ClassDB::bind_method(D_METHOD("begin_stroke", "layer_id", "x", "y", "pressure"), &DrawingBridge::begin_stroke);
    ClassDB::bind_method(D_METHOD("append_stroke_point", "stroke_id", "x", "y", "pressure"), &DrawingBridge::append_stroke_point);
    ClassDB::bind_method(D_METHOD("end_stroke", "stroke_id"), &DrawingBridge::end_stroke);
    ClassDB::bind_method(D_METHOD("erase_near", "layer_id", "x", "y", "radius"), &DrawingBridge::erase_near);
    ClassDB::bind_method(D_METHOD("clear_canvas"), &DrawingBridge::clear_canvas);

    ClassDB::bind_method(D_METHOD("get_stroke_count"), &DrawingBridge::get_stroke_count);
    ClassDB::bind_method(D_METHOD("get_stroke_points", "stroke_id"), &DrawingBridge::get_stroke_points);
    ClassDB::bind_method(D_METHOD("get_stroke_color", "stroke_id"), &DrawingBridge::get_stroke_color);
    ClassDB::bind_method(D_METHOD("get_stroke_width", "stroke_id"), &DrawingBridge::get_stroke_width);

    ClassDB::bind_method(D_METHOD("select_strokes_in_rect", "min_x", "min_y", "max_x", "max_y"), &DrawingBridge::select_strokes_in_rect);
    ClassDB::bind_method(D_METHOD("translate_strokes", "stroke_ids", "dx", "dy"), &DrawingBridge::translate_strokes);
    ClassDB::bind_method(D_METHOD("scale_strokes", "stroke_ids", "sx", "sy", "pivot_x", "pivot_y"), &DrawingBridge::scale_strokes);
    ClassDB::bind_method(D_METHOD("rotate_strokes", "stroke_ids", "angle_rad", "pivot_x", "pivot_y"), &DrawingBridge::rotate_strokes);

    ClassDB::bind_method(D_METHOD("can_undo"), &DrawingBridge::can_undo);
    ClassDB::bind_method(D_METHOD("can_redo"), &DrawingBridge::can_redo);
    ClassDB::bind_method(D_METHOD("undo"), &DrawingBridge::undo);
    ClassDB::bind_method(D_METHOD("redo"), &DrawingBridge::redo);
    ClassDB::bind_method(D_METHOD("load_sample_stickman"), &DrawingBridge::load_sample_stickman);
    ClassDB::bind_method(D_METHOD("load_sample_car"), &DrawingBridge::load_sample_car);
    ClassDB::bind_method(D_METHOD("load_sample_fish_and_ocean"), &DrawingBridge::load_sample_fish_and_ocean);
    ClassDB::bind_method(D_METHOD("load_sample_bird_and_sky"), &DrawingBridge::load_sample_bird_and_sky);
    ClassDB::bind_method(D_METHOD("load_sample_city"), &DrawingBridge::load_sample_city);
    ClassDB::bind_method(D_METHOD("commit_unassigned_strokes_to_object", "object_id", "part_id"), &DrawingBridge::commit_unassigned_strokes_to_object);
    ClassDB::bind_method(D_METHOD("get_unassigned_stroke_count"), &DrawingBridge::get_unassigned_stroke_count);
    ClassDB::bind_method(D_METHOD("serialize_canvas_json"), &DrawingBridge::serialize_canvas_json);
}

} // namespace living_canvas::gdext

