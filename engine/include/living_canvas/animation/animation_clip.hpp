#ifndef LIVING_CANVAS_ANIMATION_ANIMATION_CLIP_HPP
#define LIVING_CANVAS_ANIMATION_ANIMATION_CLIP_HPP

#include "living_canvas/animation/keyframe.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace living_canvas::animation {

struct AnimationTrack {
    int64_t track_id = 0;
    std::string target_bone_name;
    std::vector<Keyframe> keyframes;

    Keyframe evaluate(float time_sec) const;
};

class AnimationClip {
public:
    AnimationClip() = default;
    AnimationClip(int64_t clip_id, const std::string& name, float duration_sec, bool loop);

    int64_t id() const noexcept { return clip_id_; }
    const std::string& name() const noexcept { return name_; }
    float duration_sec() const noexcept { return duration_sec_; }
    bool is_looping() const noexcept { return loop_; }
    void set_looping(bool loop) noexcept { loop_ = loop; }

    void add_keyframe(const std::string& bone_name, const Keyframe& kf);
    bool remove_keyframe(const std::string& bone_name, float time_sec, float tolerance = 0.02f);
    Keyframe evaluate_bone(const std::string& bone_name, float time_sec) const;

    const std::vector<AnimationTrack>& tracks() const noexcept { return tracks_; }
    std::string to_json() const;

private:
    int64_t clip_id_ = 0;
    std::string name_ = "idle";
    float duration_sec_ = 1.0f;
    bool loop_ = true;
    std::vector<AnimationTrack> tracks_;
    int64_t next_track_id_ = 1;
};

} // namespace living_canvas::animation

#endif // LIVING_CANVAS_ANIMATION_ANIMATION_CLIP_HPP

