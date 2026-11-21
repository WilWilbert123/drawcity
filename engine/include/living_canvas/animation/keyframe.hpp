#ifndef LIVING_CANVAS_ANIMATION_KEYFRAME_HPP
#define LIVING_CANVAS_ANIMATION_KEYFRAME_HPP

#include <cstdint>
#include <string>

namespace living_canvas::animation {

enum class EasingType : int32_t {
    Linear = 0,
    SmoothStep = 1,
    CubicInOut = 2
};

struct Keyframe {
    float time_sec = 0.0f;
    float offset_x = 0.0f;
    float offset_y = 0.0f;
    float rotation_deg = 0.0f;
    float scale_x = 1.0f;
    float scale_y = 1.0f;
    EasingType easing = EasingType::SmoothStep;

    static Keyframe interpolate(const Keyframe& a, const Keyframe& b, float time_sec);
    std::string to_json() const;
};

} // namespace living_canvas::animation

#endif // LIVING_CANVAS_ANIMATION_KEYFRAME_HPP

