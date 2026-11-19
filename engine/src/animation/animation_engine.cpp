#include "living_canvas/animation/animation_engine.hpp"

#include <algorithm>

namespace living_canvas::animation {

AnimationEngine::AnimationEngine() {
    register_builtin_presets();
}

int64_t AnimationEngine::create_clip(const std::string& name, float duration_sec, bool loop) {
    for (const auto& c : clips_) {
        if (c.name() == name) {
            return c.id();
        }
    }
    const int64_t id = next_clip_id_++;
    clips_.emplace_back(id, name, duration_sec, loop);
    return id;
}

const AnimationClip* AnimationEngine::find_clip(const std::string& name) const {
    for (const auto& c : clips_) {
        if (c.name() == name) {
            return &c;
        }
    }
    return nullptr;
}

AnimationClip* AnimationEngine::find_mutable_clip(const std::string& name) {
    for (auto& c : clips_) {
        if (c.name() == name) {
            return &c;
        }
    }
    return nullptr;
}

void AnimationEngine::play(const std::string& clip_name) {
    if (find_clip(clip_name)) {
        if (active_clip_name_ != clip_name) {
            active_clip_name_ = clip_name;
            current_time_sec_ = 0.0f;
        }
        state_ = PlaybackState::Playing;
    }
}

void AnimationEngine::pause() {
    if (state_ == PlaybackState::Playing) {
        state_ = PlaybackState::Paused;
    }
}

void AnimationEngine::stop() {
    state_ = PlaybackState::Stopped;
    current_time_sec_ = 0.0f;
}

void AnimationEngine::step(float delta_sec, living_canvas::rigging::RigInstance* active_rig) {
    if (state_ != PlaybackState::Playing) {
        return;
    }
    const auto* clip = find_clip(active_clip_name_);
    if (!clip) {
        return;
    }
    current_time_sec_ += delta_sec * playback_speed_;
    if (!clip->is_looping() && current_time_sec_ >= clip->duration_sec()) {
        current_time_sec_ = clip->duration_sec();
        state_ = PlaybackState::Stopped;
    }

    if (active_rig) {
        for (auto& bone : active_rig->bones) {
            const Keyframe kf = clip->evaluate_bone(bone.name(), current_time_sec_);
            bone.set_local_rotation_deg(kf.rotation_deg);
        }
    }
}

void AnimationEngine::register_builtin_presets() {
    // 1. Idle breathing preset
    create_clip("idle", 1.6f, true);
    if (auto* idle = find_mutable_clip("idle")) {
        idle->add_keyframe("Body", Keyframe{0.0f, 0.0f, 0.0f, -90.0f, 1.0f, 1.0f});
        idle->add_keyframe("Body", Keyframe{0.8f, 0.0f, -3.0f, -88.0f, 1.0f, 1.02f});
        idle->add_keyframe("Body", Keyframe{1.6f, 0.0f, 0.0f, -90.0f, 1.0f, 1.0f});
    }

    // 2. Walk cycle preset
    create_clip("walk", 0.8f, true);
    if (auto* walk = find_mutable_clip("walk")) {
        walk->add_keyframe("LeftUpperLeg", Keyframe{0.0f, 0.0f, 0.0f, 65.0f, 1.0f, 1.0f});
        walk->add_keyframe("LeftUpperLeg", Keyframe{0.4f, 0.0f, 0.0f, 115.0f, 1.0f, 1.0f});
        walk->add_keyframe("LeftUpperLeg", Keyframe{0.8f, 0.0f, 0.0f, 65.0f, 1.0f, 1.0f});

        walk->add_keyframe("RightUpperLeg", Keyframe{0.0f, 0.0f, 0.0f, 115.0f, 1.0f, 1.0f});
        walk->add_keyframe("RightUpperLeg", Keyframe{0.4f, 0.0f, 0.0f, 65.0f, 1.0f, 1.0f});
        walk->add_keyframe("RightUpperLeg", Keyframe{0.8f, 0.0f, 0.0f, 115.0f, 1.0f, 1.0f});

        walk->add_keyframe("LeftUpperArm", Keyframe{0.0f, 0.0f, 0.0f, 120.0f, 1.0f, 1.0f});
        walk->add_keyframe("LeftUpperArm", Keyframe{0.4f, 0.0f, 0.0f, 60.0f, 1.0f, 1.0f});
        walk->add_keyframe("LeftUpperArm", Keyframe{0.8f, 0.0f, 0.0f, 120.0f, 1.0f, 1.0f});

        walk->add_keyframe("RightUpperArm", Keyframe{0.0f, 0.0f, 0.0f, 60.0f, 1.0f, 1.0f});
        walk->add_keyframe("RightUpperArm", Keyframe{0.4f, 0.0f, 0.0f, 120.0f, 1.0f, 1.0f});
        walk->add_keyframe("RightUpperArm", Keyframe{0.8f, 0.0f, 0.0f, 60.0f, 1.0f, 1.0f});
    }

    // 3. Run cycle preset
    create_clip("run", 0.5f, true);
    if (auto* run = find_mutable_clip("run")) {
        run->add_keyframe("LeftUpperLeg", Keyframe{0.0f, 0.0f, 0.0f, 45.0f, 1.0f, 1.0f});
        run->add_keyframe("LeftUpperLeg", Keyframe{0.25f, 0.0f, 0.0f, 135.0f, 1.0f, 1.0f});
        run->add_keyframe("LeftUpperLeg", Keyframe{0.5f, 0.0f, 0.0f, 45.0f, 1.0f, 1.0f});

        run->add_keyframe("RightUpperLeg", Keyframe{0.0f, 0.0f, 0.0f, 135.0f, 1.0f, 1.0f});
        run->add_keyframe("RightUpperLeg", Keyframe{0.25f, 0.0f, 0.0f, 45.0f, 1.0f, 1.0f});
        run->add_keyframe("RightUpperLeg", Keyframe{0.5f, 0.0f, 0.0f, 135.0f, 1.0f, 1.0f});
    }

    // 4. Jump preset
    create_clip("jump", 0.7f, false);
    if (auto* jump = find_mutable_clip("jump")) {
        jump->add_keyframe("LeftUpperArm", Keyframe{0.0f, 0.0f, 0.0f, 95.0f, 1.0f, 1.0f});
        jump->add_keyframe("LeftUpperArm", Keyframe{0.35f, 0.0f, -15.0f, -135.0f, 1.0f, 1.0f});
        jump->add_keyframe("LeftUpperArm", Keyframe{0.7f, 0.0f, 0.0f, 80.0f, 1.0f, 1.0f});

        jump->add_keyframe("RightUpperArm", Keyframe{0.0f, 0.0f, 0.0f, 85.0f, 1.0f, 1.0f});
        jump->add_keyframe("RightUpperArm", Keyframe{0.35f, 0.0f, -15.0f, -45.0f, 1.0f, 1.0f});
        jump->add_keyframe("RightUpperArm", Keyframe{0.7f, 0.0f, 0.0f, 100.0f, 1.0f, 1.0f});
    }

    // 5. Fall, Fly, Swim, Attack, Death presets
    create_clip("fall", 0.6f, true);
    create_clip("fly", 0.6f, true);
    if (auto* fly = find_mutable_clip("fly")) {
        fly->add_keyframe("LeftWing", Keyframe{0.0f, 0.0f, 0.0f, -35.0f, 1.0f, 1.0f});
        fly->add_keyframe("LeftWing", Keyframe{0.3f, 0.0f, 0.0f, 35.0f, 1.0f, 1.0f});
        fly->add_keyframe("LeftWing", Keyframe{0.6f, 0.0f, 0.0f, -35.0f, 1.0f, 1.0f});
        fly->add_keyframe("RightWing", Keyframe{0.0f, 0.0f, 0.0f, 35.0f, 1.0f, 1.0f});
        fly->add_keyframe("RightWing", Keyframe{0.3f, 0.0f, 0.0f, -35.0f, 1.0f, 1.0f});
        fly->add_keyframe("RightWing", Keyframe{0.6f, 0.0f, 0.0f, 35.0f, 1.0f, 1.0f});
    }
    create_clip("swim", 0.7f, true);
    if (auto* swim = find_mutable_clip("swim")) {
        swim->add_keyframe("Tail", Keyframe{0.0f, 0.0f, 0.0f, -25.0f, 1.0f, 1.0f});
        swim->add_keyframe("Tail", Keyframe{0.35f, 0.0f, 0.0f, 25.0f, 1.0f, 1.0f});
        swim->add_keyframe("Tail", Keyframe{0.7f, 0.0f, 0.0f, -25.0f, 1.0f, 1.0f});
    }
    create_clip("attack", 0.45f, false);
    create_clip("death", 1.0f, false);
}

} // namespace living_canvas::animation

