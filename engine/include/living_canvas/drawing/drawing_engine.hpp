#ifndef LIVING_CANVAS_DRAWING_DRAWING_ENGINE_HPP
#define LIVING_CANVAS_DRAWING_DRAWING_ENGINE_HPP

#include "living_canvas/drawing/brush_engine.hpp"
#include "living_canvas/drawing/stroke.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace living_canvas::drawing {

struct LayerData {
    int64_t layer_id = 1;
    std::string name = "Layer 1";
    int32_t z_index = 0;
    float opacity = 1.0f;
    bool visible = true;
    bool locked = false;
};

class DrawingEngine {
public:
    DrawingEngine();
    ~DrawingEngine() = default;

    int64_t create_layer(const std::string& name);
    bool remove_layer(int64_t layer_id);
    bool set_layer_visibility(int64_t layer_id, bool visible);
    bool set_layer_locked(int64_t layer_id, bool locked);
    const std::vector<LayerData>& layers() const noexcept { return layers_; }

    int64_t begin_stroke(int64_t layer_id, float x, float y, float pressure = 1.0f);
    void append_stroke_point(int64_t stroke_id, float x, float y, float pressure = 1.0f);
    bool end_stroke(int64_t stroke_id);

    int32_t erase_near(int64_t layer_id, float x, float y, float radius);
    void clear_canvas();

    std::vector<int64_t> select_strokes_in_rect(float min_x, float min_y, float max_x, float max_y) const;
    void translate_strokes(const std::vector<int64_t>& stroke_ids, float dx, float dy);
    void scale_strokes(const std::vector<int64_t>& stroke_ids, float sx, float sy, float pivot_x, float pivot_y);
    void rotate_strokes(const std::vector<int64_t>& stroke_ids, float angle_radians, float pivot_x, float pivot_y);
    void assign_strokes_to_object(const std::vector<int64_t>& stroke_ids, int64_t object_id, int64_t part_id = 0);

    const Stroke* find_stroke(int64_t stroke_id) const;
    Stroke* find_mutable_stroke(int64_t stroke_id);
    const std::vector<Stroke>& strokes() const noexcept { return strokes_; }
    void set_strokes(std::vector<Stroke> strokes);

    BrushEngine& brush_engine() noexcept { return brush_engine_; }
    const BrushEngine& brush_engine() const noexcept { return brush_engine_; }

    std::string serialize_canvas_json() const;

private:
    BrushEngine brush_engine_;
    std::vector<LayerData> layers_;
    std::vector<Stroke> strokes_;
    int64_t next_layer_id_ = 1;
    int64_t next_stroke_id_ = 1;
};

} // namespace living_canvas::drawing

#endif // LIVING_CANVAS_DRAWING_DRAWING_ENGINE_HPP

