#ifndef LIVING_CANVAS_GDEXT_RECOGNITION_BRIDGE_HPP
#define LIVING_CANVAS_GDEXT_RECOGNITION_BRIDGE_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/string.hpp>

#include "drawing/drawing_bridge.hpp"
#include "living_canvas/recognition/object_classifier.hpp"

namespace living_canvas::gdext {

class RecognitionBridge : public godot::RefCounted {
    GDCLASS(RecognitionBridge, godot::RefCounted)

public:
    RecognitionBridge() = default;
    ~RecognitionBridge() override = default;

    godot::String classify_drawing(DrawingBridge* drawing_bridge) const;
    godot::PackedStringArray get_supported_categories() const;

protected:
    static void _bind_methods();
};

} // namespace living_canvas::gdext

#endif // LIVING_CANVAS_GDEXT_RECOGNITION_BRIDGE_HPP
