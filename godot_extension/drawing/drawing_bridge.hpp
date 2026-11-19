#ifndef LIVING_CANVAS_GDEXT_DRAWING_BRIDGE_HPP
#define LIVING_CANVAS_GDEXT_DRAWING_BRIDGE_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/packed_int64_array.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>
#include <godot_cpp/variant/string.hpp>

#include "living_canvas/drawing/drawing_engine.hpp"
#include "living_canvas/history/history_manager.hpp"

namespace living_canvas::gdext {

class DrawingBridge : public godot::RefCounted {
    GDCLASS(DrawingBridge, godot::RefCounted)

public:
    DrawingBridge();
    ~DrawingBridge() override = default;

    int64_t create_layer(const godot::String& name);
    bool set_layer_visibility(int64_t layer_id, bool visible);
    bool set_layer_locked(int64_t layer_id, bool locked);
    int32_t get_layer_count() const;

    void select_brush(int32_t brush_type);
    void set_brush_size(double size);
    double get_brush_size() const;
    void set_brush_color(const godot::String& color_hex);
    godot::String get_brush_color() const;

    int64_t begin_stroke(int64_t layer_id, double x, double y, double pressure);
    void append_stroke_point(int64_t stroke_id, double x, double y, double pressure);
    bool end_stroke(int64_t stroke_id);
    int32_t erase_near(int64_t layer_id, double x, double y, double radius);
    void clear_canvas();

    int32_t get_stroke_count() const;
    godot::PackedVector2Array get_stroke_points(int64_t stroke_id) const;
    godot::String get_stroke_color(int64_t stroke_id) const;
    double get_stroke_width(int64_t stroke_id) const;

    godot::PackedInt64Array select_strokes_in_rect(double min_x, double min_y, double max_x, double max_y) const;
    void translate_strokes(const godot::PackedInt64Array& stroke_ids, double dx, double dy);
    void scale_strokes(const godot::PackedInt64Array& stroke_ids, double sx, double sy, double pivot_x, double pivot_y);
    void rotate_strokes(const godot::PackedInt64Array& stroke_ids, double angle_rad, double pivot_x, double pivot_y);

    bool can_undo() const;
    bool can_redo() const;
    godot::String undo();
    godot::String redo();

    void load_sample_stickman();
    void load_sample_car();
    void load_sample_fish_and_ocean();
    void load_sample_bird_and_sky();

    godot::String serialize_canvas_json() const;

    living_canvas::drawing::DrawingEngine& native_engine() noexcept { return engine_; }
    const living_canvas::drawing::DrawingEngine& native_engine() const noexcept { return engine_; }

protected:
    static void _bind_methods();

private:
    living_canvas::drawing::DrawingEngine engine_;
    living_canvas::history::HistoryManager history_;
};

} // namespace living_canvas::gdext

#endif // LIVING_CANVAS_GDEXT_DRAWING_BRIDGE_HPP

