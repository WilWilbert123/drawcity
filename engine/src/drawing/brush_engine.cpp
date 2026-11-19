#include "living_canvas/drawing/brush_engine.hpp"

#include <algorithm>

namespace living_canvas::drawing {

BrushEngine::BrushEngine() {
    presets_ = {
        BrushDescriptor{BrushType::Pencil, "Pencil", 4.0f, 1.0f, 32.0f, 1.0f, 0.35f, true},
        BrushDescriptor{BrushType::Pen, "Pen", 5.0f, 1.0f, 48.0f, 1.0f, 0.50f, true},
        BrushDescriptor{BrushType::Marker, "Marker", 14.0f, 4.0f, 64.0f, 0.78f, 0.45f, false},
        BrushDescriptor{BrushType::SoftBrush, "Soft Brush", 18.0f, 4.0f, 64.0f, 0.65f, 0.60f, true},
        BrushDescriptor{BrushType::Eraser, "Eraser", 20.0f, 2.0f, 96.0f, 1.0f, 0.25f, false}
    };
    current_brush_ = presets_.front();
}

void BrushEngine::select_brush(BrushType type) {
    for (const auto& preset : presets_) {
        if (preset.type == type) {
            current_brush_ = preset;
            return;
        }
    }
}

void BrushEngine::set_brush_size(float size) {
    current_brush_.default_size = std::max(current_brush_.min_size, std::min(current_brush_.max_size, size));
}

void BrushEngine::set_brush_opacity(float opacity) {
    current_brush_.opacity = std::max(0.05f, std::min(1.0f, opacity));
}

void BrushEngine::set_brush_smoothing(float smoothing) {
    current_brush_.smoothing = std::max(0.0f, std::min(0.95f, smoothing));
}

void BrushEngine::set_color_hex(const std::string& color_hex) {
    if (!color_hex.empty()) {
        current_color_hex_ = color_hex;
    }
}

float BrushEngine::compute_effective_width(float pressure) const {
    if (!current_brush_.pressure_sensitive) {
        return current_brush_.default_size;
    }
    const float clamped = std::max(0.15f, std::min(1.0f, pressure));
    return current_brush_.default_size * (0.35f + 0.65f * clamped);
}

} // namespace living_canvas::drawing

