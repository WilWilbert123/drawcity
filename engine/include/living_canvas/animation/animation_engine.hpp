#ifndef LIVING_CANVAS_ANIMATION_ANIMATION_ENGINE_HPP
#define LIVING_CANVAS_ANIMATION_ANIMATION_ENGINE_HPP

#include "living_canvas/animation/animation_clip.hpp"
#include "living_canvas/rigging/rig_manager.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace living_canvas::animation {

enum class PlaybackState : int32_t {
    Stopped = 0,
    Playing = 1,
    Paused = 2
};

class AnimationEngine {
public:
    AnimationEngine();
    ~AnimationEngine() = default;

    int64_t create_clip(const std::string& name, float duration_sec, bool loop);
    const AnimationClip* find_clip(const std::string& name) const;
    AnimationClip* find_mutable_clip(const std::string& name);

    void play(const std::string& clip_name);
    void pause();
    void stop();
    void set_speed(float speed) noexcept { playback_speed_ = speed; }
    void step(float delta_sec, living_canvas::rigging::RigInstance* active_rig = nullptr);

    PlaybackState state() const noexcept { return state_; }
    float current_time_sec() const noexcept { return current_time_sec_; }
    const std::string& active_clip_name() const noexcept { return active_clip_name_; }
    const std::vector<AnimationClip>& clips() const noexcept { return clips_; }

private:
    void register_builtin_presets();

    std::vector<AnimationClip> clips_;
    int64_t next_clip_id_ = 1;
    std::string active_clip_name_ = "idle";
    PlaybackState state_ = PlaybackState::Stopped;
    float current_time_sec_ = 0.0f;
    float playback_speed_ = 1.0f;
};

} // namespace living_canvas::animation

#endif // LIVING_CANVAS_ANIMATION_ANIMATION_ENGINE_HPP

