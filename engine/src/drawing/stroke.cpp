#include "living_canvas/drawing/stroke.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <sstream>

namespace living_canvas::drawing {

Stroke::Stroke(int64_t stroke_id, int64_t layer_id, BrushType brush_type, const std::string& color_hex, float base_width)
    : stroke_id_(stroke_id),
      layer_id_(layer_id),
      brush_type_(brush_type),
      color_hex_(color_hex),
      base_width_(base_width) {}

void Stroke::add_point(float x, float y, float pressure, int64_t timestamp_ms) {
    if (locked_) {
        return;
    }
    const float clamped_pressure = std::max(0.05f, std::min(1.0f, pressure));
    raw_points_.push_back(StrokePoint{x, y, clamped_pressure, timestamp_ms});
    smoothed_points_.clear();
}

void Stroke::clear_points() {
    if (locked_) {
        return;
    }
    raw_points_.clear();
    smoothed_points_.clear();
}

void Stroke::apply_smoothing(float factor) {
    const float alpha = std::max(0.0f, std::min(0.95f, factor));
    if (raw_points_.size() <= 2 || alpha <= 0.001f) {
        smoothed_points_ = raw_points_;
        return;
    }

    smoothed_points_.resize(raw_points_.size());
    smoothed_points_.front() = raw_points_.front();
    for (std::size_t i = 1; i + 1 < raw_points_.size(); ++i) {
        const auto& prev = raw_points_[i - 1];
        const auto& curr = raw_points_[i];
        const auto& next = raw_points_[i + 1];
        const float avg_x = (prev.x + curr.x + next.x) / 3.0f;
        const float avg_y = (prev.y + curr.y + next.y) / 3.0f;
        const float avg_p = (prev.pressure + curr.pressure + next.pressure) / 3.0f;
        smoothed_points_[i].x = curr.x * (1.0f - alpha) + avg_x * alpha;
        smoothed_points_[i].y = curr.y * (1.0f - alpha) + avg_y * alpha;
        smoothed_points_[i].pressure = curr.pressure * (1.0f - alpha) + avg_p * alpha;
        smoothed_points_[i].timestamp_ms = curr.timestamp_ms;
    }
    smoothed_points_.back() = raw_points_.back();
}

void Stroke::translate(float dx, float dy) {
    if (locked_) {
        return;
    }
    for (auto& pt : raw_points_) {
        pt.x += dx;
        pt.y += dy;
    }
    for (auto& pt : smoothed_points_) {
        pt.x += dx;
        pt.y += dy;
    }
}

void Stroke::scale(float sx, float sy, float pivot_x, float pivot_y) {
    if (locked_) {
        return;
    }
    for (auto& pt : raw_points_) {
        pt.x = pivot_x + (pt.x - pivot_x) * sx;
        pt.y = pivot_y + (pt.y - pivot_y) * sy;
    }
    for (auto& pt : smoothed_points_) {
        pt.x = pivot_x + (pt.x - pivot_x) * sx;
        pt.y = pivot_y + (pt.y - pivot_y) * sy;
    }
}

void Stroke::rotate(float angle_radians, float pivot_x, float pivot_y) {
    if (locked_) {
        return;
    }
    const float c = std::cos(angle_radians);
    const float s = std::sin(angle_radians);
    auto rotate_vec = [&](std::vector<StrokePoint>& pts) {
        for (auto& pt : pts) {
            const float rx = pt.x - pivot_x;
            const float ry = pt.y - pivot_y;
            pt.x = pivot_x + (rx * c - ry * s);
            pt.y = pivot_y + (rx * s + ry * c);
        }
    };
    rotate_vec(raw_points_);
    rotate_vec(smoothed_points_);
}

BoundingBox Stroke::compute_bounds() const {
    BoundingBox box;
    if (raw_points_.empty()) {
        return box;
    }
    box.min_x = raw_points_.front().x;
    box.max_x = raw_points_.front().x;
    box.min_y = raw_points_.front().y;
    box.max_y = raw_points_.front().y;
    box.valid = true;
    for (const auto& pt : raw_points_) {
        box.min_x = std::min(box.min_x, pt.x);
        box.max_x = std::max(box.max_x, pt.x);
        box.min_y = std::min(box.min_y, pt.y);
        box.max_y = std::max(box.max_y, pt.y);
    }
    return box;
}

float Stroke::total_length() const {
    if (raw_points_.size() < 2) {
        return 0.0f;
    }
    float length = 0.0f;
    for (std::size_t i = 1; i < raw_points_.size(); ++i) {
        const float dx = raw_points_[i].x - raw_points_[i - 1].x;
        const float dy = raw_points_[i].y - raw_points_[i - 1].y;
        length += std::sqrt(dx * dx + dy * dy);
    }
    return length;
}

std::vector<uint8_t> Stroke::serialize_points_blob() const {
    std::vector<uint8_t> blob(raw_points_.size() * sizeof(StrokePoint));
    if (!raw_points_.empty()) {
        std::memcpy(blob.data(), raw_points_.data(), blob.size());
    }
    return blob;
}

bool Stroke::deserialize_points_blob(const uint8_t* data, std::size_t size_bytes) {
    if (size_bytes == 0) {
        raw_points_.clear();
        smoothed_points_.clear();
        return true;
    }
    if (!data || (size_bytes % sizeof(StrokePoint)) != 0) {
        return false;
    }
    const std::size_t count = size_bytes / sizeof(StrokePoint);
    raw_points_.resize(count);
    std::memcpy(raw_points_.data(), data, size_bytes);
    smoothed_points_.clear();
    return true;
}

std::string Stroke::to_json() const {
    std::ostringstream out;
    out << "{"
        << "\"id\":" << stroke_id_ << ","
        << "\"layer_id\":" << layer_id_ << ","
        << "\"object_id\":" << object_id_ << ","
        << "\"part_id\":" << part_id_ << ","
        << "\"brush_type\":" << static_cast<int32_t>(brush_type_) << ","
        << "\"color\":\"" << color_hex_ << "\","
        << "\"width\":" << base_width_ << ","
        << "\"order\":" << stroke_order_ << ","
        << "\"visible\":" << (visible_ ? "true" : "false") << ","
        << "\"locked\":" << (locked_ ? "true" : "false") << ","
        << "\"points\":[";
    for (std::size_t i = 0; i < raw_points_.size(); ++i) {
        if (i > 0) {
            out << ",";
        }
        out << "[" << raw_points_[i].x << "," << raw_points_[i].y << "," << raw_points_[i].pressure << "]";
    }
    out << "]}";
    return out.str();
}

} // namespace living_canvas::drawing

