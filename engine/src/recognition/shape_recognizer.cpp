#include "living_canvas/recognition/shape_recognizer.hpp"

#include <algorithm>
#include <cmath>

namespace living_canvas::recognition {

StrokeShapeDescriptor ShapeRecognizer::analyze_stroke(const living_canvas::drawing::Stroke& stroke) {
    StrokeShapeDescriptor desc;
    desc.bounds = stroke.compute_bounds();
    const auto& pts = stroke.raw_points();
    if (pts.size() < 2 || !desc.bounds.valid) {
        return desc;
    }

    const float w = std::max(1.0f, desc.bounds.width());
    const float h = std::max(1.0f, desc.bounds.height());
    desc.aspect_ratio = w / h;

    const float path_len = std::max(1.0f, stroke.total_length());
    const float end_dx = pts.back().x - pts.front().x;
    const float end_dy = pts.back().y - pts.front().y;
    const float end_dist = std::sqrt(end_dx * end_dx + end_dy * end_dy);

    desc.straightness = std::min(1.0f, end_dist / path_len);
    const float diag = std::sqrt(w * w + h * h);
    desc.is_closed = (pts.size() >= 6 && end_dist <= diag * 0.32f);

    if (desc.is_closed && desc.aspect_ratio >= 0.55f && desc.aspect_ratio <= 1.8f) {
        desc.circularity = 1.0f - std::min(1.0f, std::fabs(1.0f - desc.aspect_ratio));
        desc.shape = PrimitiveShape::Circle;
    } else if (desc.straightness >= 0.85f) {
        desc.shape = PrimitiveShape::Line;
    } else if (desc.aspect_ratio >= 3.5f && !desc.is_closed) {
        desc.shape = PrimitiveShape::Wave;
    } else if (desc.is_closed) {
        desc.shape = PrimitiveShape::Polygon;
    } else {
        desc.shape = PrimitiveShape::Arc;
    }
    return desc;
}

std::string ShapeRecognizer::shape_to_string(PrimitiveShape shape) {
    switch (shape) {
        case PrimitiveShape::Circle: return "circle";
        case PrimitiveShape::Line: return "line";
        case PrimitiveShape::Arc: return "arc";
        case PrimitiveShape::Polygon: return "polygon";
        case PrimitiveShape::Wave: return "wave";
        default: return "unknown";
    }
}

} // namespace living_canvas::recognition

