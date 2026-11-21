#include "recognition/recognition_bridge.hpp"

#include <godot_cpp/core/class_db.hpp>

namespace living_canvas::gdext {

godot::String RecognitionBridge::classify_drawing(DrawingBridge* drawing_bridge) const {
    if (drawing_bridge == nullptr) {
        return godot::String("{\"category\":\"other\",\"confidence\":0.0,\"requires_manual_confirmation\":true}");
    }
    const auto res = living_canvas::recognition::ObjectClassifier::classify_strokes(
        drawing_bridge->native_engine().strokes());
    return godot::String(res.to_json().c_str());
}

godot::PackedStringArray RecognitionBridge::get_supported_categories() const {
    godot::PackedStringArray arr;
    for (const auto& cat : living_canvas::recognition::ObjectClassifier::supported_categories()) {
        arr.push_back(godot::String(cat.c_str()));
    }
    return arr;
}

void RecognitionBridge::_bind_methods() {
    using namespace godot;
    ClassDB::bind_method(D_METHOD("classify_drawing", "drawing_bridge"), &RecognitionBridge::classify_drawing);
    ClassDB::bind_method(D_METHOD("get_supported_categories"), &RecognitionBridge::get_supported_categories);
}

} // namespace living_canvas::gdext
