#ifndef LIVING_CANVAS_DRAWING_STROKE_HPP
#define LIVING_CANVAS_DRAWING_STROKE_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace living_canvas::drawing {

struct StrokePoint {
    float x = 0.0f;
    float y = 0.0f;
    float pressure = 1.0f;
    int64_t timestamp_ms = 0;
};

struct BoundingBox {
    float min_x = 0.0f;
    float min_y = 0.0f;
    float max_x = 0.0f;
    float max_y = 0.0f;
    bool valid = false;

    float width() const noexcept { return valid ? (max_x - min_x) : 0.0f; }
    float height() const noexcept { return valid ? (max_y - min_y) : 0.0f; }
    float center_x() const noexcept { return valid ? (min_x + max_x) * 0.5f : 0.0f; }
    float center_y() const noexcept { return valid ? (min_y + max_y) * 0.5f : 0.0f; }
};

enum class BrushType : int32_t {
    Pencil = 0,
    Pen = 1,
    Marker = 2,
    SoftBrush = 3,
    Eraser = 4
};

class Stroke {
public:
    Stroke() = default;
    Stroke(int64_t stroke_id, int64_t layer_id, BrushType brush_type, const std::string& color_hex, float base_width);

    void add_point(float x, float y, float pressure = 1.0f, int64_t timestamp_ms = 0);
    void clear_points();

    void apply_smoothing(float factor);
    void translate(float dx, float dy);
    void scale(float sx, float sy, float pivot_x, float pivot_y);
    void rotate(float angle_radians, float pivot_x, float pivot_y);

    BoundingBox compute_bounds() const;
    float total_length() const;
    std::vector<uint8_t> serialize_points_blob() const;
    bool deserialize_points_blob(const uint8_t* data, std::size_t size_bytes);
    std::string to_json() const;

    int64_t id() const noexcept { return stroke_id_; }
    void set_id(int64_t id) noexcept { stroke_id_ = id; }

    int64_t layer_id() const noexcept { return layer_id_; }
    void set_layer_id(int64_t layer_id) noexcept { layer_id_ = layer_id; }

    int64_t object_id() const noexcept { return object_id_; }
    void set_object_id(int64_t object_id) noexcept { object_id_ = object_id; }

    int64_t part_id() const noexcept { return part_id_; }
    void set_part_id(int64_t part_id) noexcept { part_id_ = part_id; }

    BrushType brush_type() const noexcept { return brush_type_; }
    void set_brush_type(BrushType type) noexcept { brush_type_ = type; }

    const std::string& color_hex() const noexcept { return color_hex_; }
    void set_color_hex(const std::string& hex) { color_hex_ = hex; }

    float base_width() const noexcept { return base_width_; }
    void set_base_width(float width) noexcept { base_width_ = width; }

    int32_t stroke_order() const noexcept { return stroke_order_; }
    void set_stroke_order(int32_t order) noexcept { stroke_order_ = order; }

    bool is_visible() const noexcept { return visible_; }
    void set_visible(bool visible) noexcept { visible_ = visible; }

    bool is_locked() const noexcept { return locked_; }
    void set_locked(bool locked) noexcept { locked_ = locked; }

    const std::vector<StrokePoint>& raw_points() const noexcept { return raw_points_; }
    const std::vector<StrokePoint>& smoothed_points() const noexcept {
        return smoothed_points_.empty() ? raw_points_ : smoothed_points_;
    }

private:
    int64_t stroke_id_ = 0;
    int64_t layer_id_ = 1;
    int64_t object_id_ = 0;
    int64_t part_id_ = 0;
    BrushType brush_type_ = BrushType::Pencil;
    std::string color_hex_ = "#F9FAFB";
    float base_width_ = 4.0f;
    int32_t stroke_order_ = 0;
    bool visible_ = true;
    bool locked_ = false;
    std::vector<StrokePoint> raw_points_;
    std::vector<StrokePoint> smoothed_points_;
};

} // namespace living_canvas::drawing

#endif // LIVING_CANVAS_DRAWING_STROKE_HPP

