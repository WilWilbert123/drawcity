#ifndef LIVING_CANVAS_DRAWING_BRUSH_ENGINE_HPP
#define LIVING_CANVAS_DRAWING_BRUSH_ENGINE_HPP

#include "living_canvas/drawing/stroke.hpp"

#include <string>
#include <vector>

namespace living_canvas::drawing {

struct BrushDescriptor {
    BrushType type = BrushType::Pencil;
    std::string name = "Pencil";
    float default_size = 4.0f;
    float min_size = 1.0f;
    float max_size = 64.0f;
    float opacity = 1.0f;
    float smoothing = 0.45f;
    bool pressure_sensitive = true;
};

class BrushEngine {
public:
    BrushEngine();
    ~BrushEngine() = default;

    const BrushDescriptor& current_brush() const noexcept { return current_brush_; }
    void select_brush(BrushType type);
    void set_brush_size(float size);
    void set_brush_opacity(float opacity);
    void set_brush_smoothing(float smoothing);
    void set_color_hex(const std::string& color_hex);
    const std::string& color_hex() const noexcept { return current_color_hex_; }

    float compute_effective_width(float pressure) const;
    const std::vector<BrushDescriptor>& presets() const noexcept { return presets_; }

private:
    std::vector<BrushDescriptor> presets_;
    BrushDescriptor current_brush_;
    std::string current_color_hex_ = "#F9FAFB";
};

} // namespace living_canvas::drawing

#endif // LIVING_CANVAS_DRAWING_BRUSH_ENGINE_HPP

