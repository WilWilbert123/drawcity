#ifndef LIVING_CANVAS_RECOGNITION_SHAPE_RECOGNIZER_HPP
#define LIVING_CANVAS_RECOGNITION_SHAPE_RECOGNIZER_HPP

#include "living_canvas/drawing/stroke.hpp"

#include <string>

namespace living_canvas::recognition {

enum class PrimitiveShape : int32_t {
    Unknown = 0,
    Circle = 1,
    Line = 2,
    Arc = 3,
    Polygon = 4,
    Wave = 5
};

struct StrokeShapeDescriptor {
    PrimitiveShape shape = PrimitiveShape::Unknown;
    float circularity = 0.0f;
    float straightness = 0.0f;
    float aspect_ratio = 1.0f;
    bool is_closed = false;
    living_canvas::drawing::BoundingBox bounds;
};

class ShapeRecognizer {
public:
    static StrokeShapeDescriptor analyze_stroke(const living_canvas::drawing::Stroke& stroke);
    static std::string shape_to_string(PrimitiveShape shape);
};

} // namespace living_canvas::recognition

#endif // LIVING_CANVAS_RECOGNITION_SHAPE_RECOGNIZER_HPP

