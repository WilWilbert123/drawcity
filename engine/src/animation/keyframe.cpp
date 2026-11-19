#include "living_canvas/animation/keyframe.hpp"

#include <algorithm>
#include <sstream>

namespace living_canvas::animation {

Keyframe Keyframe::interpolate(const Keyframe& a, const Keyframe& b, float time_sec) {
    if (b.time_sec <= a.time_sec + 1e-5f) {
        return a;
    }
    float t = (time_sec - a.time_sec) / (b.time_sec - a.time_sec);
    t = std::max(0.0f, std::min(1.0f, t));

    if (a.easing == EasingType::SmoothStep) {
        t = t * t * (3.0f - 2.0f * t);
    } else if (a.easing == EasingType::CubicInOut) {
        t = (t < 0.5f) ? (4.0f * t * t * t) : (1.0f - (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) * (-2.0f * t + 2.0f) * 0.5f);
    }

    Keyframe result;
    result.time_sec = time_sec;
    result.offset_x = a.offset_x + (b.offset_x - a.offset_x) * t;
    result.offset_y = a.offset_y + (b.offset_y - a.offset_y) * t;
    result.rotation_deg = a.rotation_deg + (b.rotation_deg - a.rotation_deg) * t;
    result.scale_x = a.scale_x + (b.scale_x - a.scale_x) * t;
    result.scale_y = a.scale_y + (b.scale_y - a.scale_y) * t;
    result.easing = a.easing;
    return result;
}

std::string Keyframe::to_json() const {
    std::ostringstream out;
    out << "{"
        << "\"time_sec\":" << time_sec << ","
        << "\"offset_x\":" << offset_x << ","
        << "\"offset_y\":" << offset_y << ","
        << "\"rotation_deg\":" << rotation_deg << ","
        << "\"scale_x\":" << scale_x << ","
        << "\"scale_y\":" << scale_y
        << "}";
    return out.str();
}

} // namespace living_canvas::animation

