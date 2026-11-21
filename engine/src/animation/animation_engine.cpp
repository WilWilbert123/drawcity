#include "living_canvas/animation/animation_engine.hpp"

#include <algorithm>
#include <cmath>

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
    if (state_ == PlaybackState::Stopped && active_rig) {
        for (auto& j : active_rig->joints) {
            j.reset_to_rest();
        }
        for (auto& b : active_rig->bones) {
            b.set_local_rotation_deg(0.0f);
        }
        return;
    }
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
        // First reset all joints to rest, then apply hierarchical relative rotation + offset
        for (auto& j : active_rig->joints) {
            j.reset_to_rest();
        }
        for (auto& bone : active_rig->bones) {
            bool has_track = false;
            for (const auto& tr : clip->tracks()) {
                if (tr.target_bone_name == bone.name()) {
                    has_track = true;
                    break;
                }
            }
            if (!has_track) {
                continue;
            }
            const Keyframe kf = clip->evaluate_bone(bone.name(), current_time_sec_);
            bone.set_local_rotation_deg(kf.rotation_deg);

            living_canvas::rigging::Joint* sj = nullptr;
            living_canvas::rigging::Joint* ej = nullptr;
            for (auto& j : active_rig->joints) {
                if (j.id() == bone.start_joint_id()) {
                    sj = &j;
                } else if (j.id() == bone.end_joint_id()) {
                    ej = &j;
                }
            }
            if (sj && ej) {
                // Relative rotation around the bone's original rest angle preserves user's drawn limb angle!
                const float rest_dx = ej->rest_x() - sj->rest_x();
                const float rest_dy = ej->rest_y() - sj->rest_y();
                const float rest_len = std::max(1.0f, std::sqrt(rest_dx * rest_dx + rest_dy * rest_dy));
                const float rest_angle = std::atan2(rest_dy, rest_dx);
                const float delta_rad = kf.rotation_deg * 3.1415926535f / 180.0f;
                const float new_angle = rest_angle + delta_rad;

                const float old_ex = ej->x();
                const float old_ey = ej->y();
                const float new_ex = sj->x() + std::cos(new_angle) * rest_len + kf.offset_x;
                const float new_ey = sj->y() + std::sin(new_angle) * rest_len + kf.offset_y;
                ej->set_position(new_ex, new_ey);

                // Propagate joint shift & rotation to any child bone connected to ej
                const float shift_x = new_ex - old_ex;
                const float shift_y = new_ey - old_ey;
                for (auto& child_bone : active_rig->bones) {
                    if (child_bone.parent_bone_id() == bone.id() && child_bone.start_joint_id() == ej->id()) {
                        for (auto& child_ej : active_rig->joints) {
                            if (child_ej.id() == child_bone.end_joint_id()) {
                                const float cdx = child_ej.rest_x() - ej->rest_x();
                                const float cdy = child_ej.rest_y() - ej->rest_y();
                                const float c_rot_x = cdx * std::cos(delta_rad) - cdy * std::sin(delta_rad);
                                const float c_rot_y = cdx * std::sin(delta_rad) + cdy * std::cos(delta_rad);
                                child_ej.set_position(new_ex + c_rot_x, new_ey + c_rot_y);
                                break;
                            }
                        }
                        (void)shift_x;
                        (void)shift_y;
                    }
                }
            }
        }
    }
}

void AnimationEngine::register_builtin_presets() {
    // All rotation_deg values are RELATIVE DELTA ANGLES (in degrees) from the user's drawn pose!

    // 1. Idle breathing preset (subtle head & arm sway)
    create_clip("idle", 1.8f, true);
    if (auto* idle = find_mutable_clip("idle")) {
        idle->add_keyframe("Head", Keyframe{0.0f, 0.0f, 0.0f, -3.0f, 1.0f, 1.0f});
        idle->add_keyframe("Head", Keyframe{0.9f, 0.0f, -4.0f, 3.0f, 1.0f, 1.0f});
        idle->add_keyframe("Head", Keyframe{1.8f, 0.0f, 0.0f, -3.0f, 1.0f, 1.0f});

        idle->add_keyframe("LeftUpperArm", Keyframe{0.0f, 0.0f, 0.0f, -5.0f, 1.0f, 1.0f});
        idle->add_keyframe("LeftUpperArm", Keyframe{0.9f, 0.0f, 0.0f, 5.0f, 1.0f, 1.0f});
        idle->add_keyframe("LeftUpperArm", Keyframe{1.8f, 0.0f, 0.0f, -5.0f, 1.0f, 1.0f});

        idle->add_keyframe("RightUpperArm", Keyframe{0.0f, 0.0f, 0.0f, 5.0f, 1.0f, 1.0f});
        idle->add_keyframe("RightUpperArm", Keyframe{0.9f, 0.0f, 0.0f, -5.0f, 1.0f, 1.0f});
        idle->add_keyframe("RightUpperArm", Keyframe{1.8f, 0.0f, 0.0f, 5.0f, 1.0f, 1.0f});
    }

    // 2. Walk cycle preset (natural alternating ±24 deg leg stride, ±18 deg arm swing, subtle head bob)
    create_clip("walk", 0.9f, true);
    if (auto* walk = find_mutable_clip("walk")) {
        walk->add_keyframe("Head", Keyframe{0.0f, 0.0f, 0.0f, -2.0f, 1.0f, 1.0f});
        walk->add_keyframe("Head", Keyframe{0.225f, 0.0f, -5.0f, 0.0f, 1.0f, 1.0f});
        walk->add_keyframe("Head", Keyframe{0.45f, 0.0f, 0.0f, 2.0f, 1.0f, 1.0f});
        walk->add_keyframe("Head", Keyframe{0.675f, 0.0f, -5.0f, 0.0f, 1.0f, 1.0f});
        walk->add_keyframe("Head", Keyframe{0.9f, 0.0f, 0.0f, -2.0f, 1.0f, 1.0f});

        walk->add_keyframe("LeftUpperLeg", Keyframe{0.0f, 0.0f, 0.0f, -24.0f, 1.0f, 1.0f});
        walk->add_keyframe("LeftUpperLeg", Keyframe{0.45f, 0.0f, 0.0f, 24.0f, 1.0f, 1.0f});
        walk->add_keyframe("LeftUpperLeg", Keyframe{0.9f, 0.0f, 0.0f, -24.0f, 1.0f, 1.0f});

        walk->add_keyframe("RightUpperLeg", Keyframe{0.0f, 0.0f, 0.0f, 24.0f, 1.0f, 1.0f});
        walk->add_keyframe("RightUpperLeg", Keyframe{0.45f, 0.0f, 0.0f, -24.0f, 1.0f, 1.0f});
        walk->add_keyframe("RightUpperLeg", Keyframe{0.9f, 0.0f, 0.0f, 24.0f, 1.0f, 1.0f});

        walk->add_keyframe("LeftUpperArm", Keyframe{0.0f, 0.0f, 0.0f, 18.0f, 1.0f, 1.0f});
        walk->add_keyframe("LeftUpperArm", Keyframe{0.45f, 0.0f, 0.0f, -18.0f, 1.0f, 1.0f});
        walk->add_keyframe("LeftUpperArm", Keyframe{0.9f, 0.0f, 0.0f, 18.0f, 1.0f, 1.0f});

        walk->add_keyframe("RightUpperArm", Keyframe{0.0f, 0.0f, 0.0f, -18.0f, 1.0f, 1.0f});
        walk->add_keyframe("RightUpperArm", Keyframe{0.45f, 0.0f, 0.0f, 18.0f, 1.0f, 1.0f});
        walk->add_keyframe("RightUpperArm", Keyframe{0.9f, 0.0f, 0.0f, -18.0f, 1.0f, 1.0f});
    }

    // 3. Run cycle preset (faster ±38 deg stride & ±32 deg arm pump)
    create_clip("run", 0.55f, true);
    if (auto* run = find_mutable_clip("run")) {
        run->add_keyframe("Head", Keyframe{0.0f, 2.0f, 0.0f, 4.0f, 1.0f, 1.0f});
        run->add_keyframe("Head", Keyframe{0.275f, 2.0f, -8.0f, -4.0f, 1.0f, 1.0f});
        run->add_keyframe("Head", Keyframe{0.55f, 2.0f, 0.0f, 4.0f, 1.0f, 1.0f});

        run->add_keyframe("LeftUpperLeg", Keyframe{0.0f, 0.0f, 0.0f, -38.0f, 1.0f, 1.0f});
        run->add_keyframe("LeftUpperLeg", Keyframe{0.275f, 0.0f, 0.0f, 38.0f, 1.0f, 1.0f});
        run->add_keyframe("LeftUpperLeg", Keyframe{0.55f, 0.0f, 0.0f, -38.0f, 1.0f, 1.0f});

        run->add_keyframe("RightUpperLeg", Keyframe{0.0f, 0.0f, 0.0f, 38.0f, 1.0f, 1.0f});
        run->add_keyframe("RightUpperLeg", Keyframe{0.275f, 0.0f, 0.0f, -38.0f, 1.0f, 1.0f});
        run->add_keyframe("RightUpperLeg", Keyframe{0.55f, 0.0f, 0.0f, 38.0f, 1.0f, 1.0f});

        run->add_keyframe("LeftUpperArm", Keyframe{0.0f, 0.0f, 0.0f, 32.0f, 1.0f, 1.0f});
        run->add_keyframe("LeftUpperArm", Keyframe{0.275f, 0.0f, 0.0f, -32.0f, 1.0f, 1.0f});
        run->add_keyframe("LeftUpperArm", Keyframe{0.55f, 0.0f, 0.0f, 32.0f, 1.0f, 1.0f});

        run->add_keyframe("RightUpperArm", Keyframe{0.0f, 0.0f, 0.0f, -32.0f, 1.0f, 1.0f});
        run->add_keyframe("RightUpperArm", Keyframe{0.275f, 0.0f, 0.0f, 32.0f, 1.0f, 1.0f});
        run->add_keyframe("RightUpperArm", Keyframe{0.55f, 0.0f, 0.0f, -32.0f, 1.0f, 1.0f});
    }

    // 4. Jump preset (looping celebratory jump with arms raising and legs tucking)
    create_clip("jump", 0.85f, true);
    if (auto* jump = find_mutable_clip("jump")) {
        jump->add_keyframe("Head", Keyframe{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f});
        jump->add_keyframe("Head", Keyframe{0.425f, 0.0f, -18.0f, 0.0f, 1.0f, 1.0f});
        jump->add_keyframe("Head", Keyframe{0.85f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f});

        jump->add_keyframe("LeftUpperArm", Keyframe{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f});
        jump->add_keyframe("LeftUpperArm", Keyframe{0.425f, 0.0f, -12.0f, 48.0f, 1.0f, 1.0f});
        jump->add_keyframe("LeftUpperArm", Keyframe{0.85f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f});

        jump->add_keyframe("RightUpperArm", Keyframe{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f});
        jump->add_keyframe("RightUpperArm", Keyframe{0.425f, 0.0f, -12.0f, -48.0f, 1.0f, 1.0f});
        jump->add_keyframe("RightUpperArm", Keyframe{0.85f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f});

        jump->add_keyframe("LeftUpperLeg", Keyframe{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f});
        jump->add_keyframe("LeftUpperLeg", Keyframe{0.425f, 0.0f, -10.0f, 22.0f, 1.0f, 1.0f});
        jump->add_keyframe("LeftUpperLeg", Keyframe{0.85f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f});

        jump->add_keyframe("RightUpperLeg", Keyframe{0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f});
        jump->add_keyframe("RightUpperLeg", Keyframe{0.425f, 0.0f, -10.0f, -22.0f, 1.0f, 1.0f});
        jump->add_keyframe("RightUpperLeg", Keyframe{0.85f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f});
    }

    // 5. Fall, Fly, Swim, Attack, Death presets
    create_clip("fall", 0.7f, true);
    if (auto* fall = find_mutable_clip("fall")) {
        fall->add_keyframe("LeftUpperArm", Keyframe{0.0f, 0.0f, 0.0f, 35.0f, 1.0f, 1.0f});
        fall->add_keyframe("LeftUpperArm", Keyframe{0.35f, 0.0f, 0.0f, 55.0f, 1.0f, 1.0f});
        fall->add_keyframe("LeftUpperArm", Keyframe{0.7f, 0.0f, 0.0f, 35.0f, 1.0f, 1.0f});

        fall->add_keyframe("RightUpperArm", Keyframe{0.0f, 0.0f, 0.0f, -35.0f, 1.0f, 1.0f});
        fall->add_keyframe("RightUpperArm", Keyframe{0.35f, 0.0f, 0.0f, -55.0f, 1.0f, 1.0f});
        fall->add_keyframe("RightUpperArm", Keyframe{0.7f, 0.0f, 0.0f, -35.0f, 1.0f, 1.0f});
    }

    create_clip("fly", 0.65f, true);
    if (auto* fly = find_mutable_clip("fly")) {
        fly->add_keyframe("LeftUpperArm", Keyframe{0.0f, 0.0f, 0.0f, -36.0f, 1.0f, 1.0f});
        fly->add_keyframe("LeftUpperArm", Keyframe{0.325f, 0.0f, -8.0f, 36.0f, 1.0f, 1.0f});
        fly->add_keyframe("LeftUpperArm", Keyframe{0.65f, 0.0f, 0.0f, -36.0f, 1.0f, 1.0f});

        fly->add_keyframe("RightUpperArm", Keyframe{0.0f, 0.0f, 0.0f, 36.0f, 1.0f, 1.0f});
        fly->add_keyframe("RightUpperArm", Keyframe{0.325f, 0.0f, -8.0f, -36.0f, 1.0f, 1.0f});
        fly->add_keyframe("RightUpperArm", Keyframe{0.65f, 0.0f, 0.0f, 36.0f, 1.0f, 1.0f});
    }

    create_clip("swim", 0.8f, true);
    if (auto* swim = find_mutable_clip("swim")) {
        swim->add_keyframe("LeftUpperArm", Keyframe{0.0f, 0.0f, 0.0f, -28.0f, 1.0f, 1.0f});
        swim->add_keyframe("LeftUpperArm", Keyframe{0.4f, 0.0f, 0.0f, 28.0f, 1.0f, 1.0f});
        swim->add_keyframe("LeftUpperArm", Keyframe{0.8f, 0.0f, 0.0f, -28.0f, 1.0f, 1.0f});

        swim->add_keyframe("RightUpperArm", Keyframe{0.0f, 0.0f, 0.0f, 28.0f, 1.0f, 1.0f});
        swim->add_keyframe("RightUpperArm", Keyframe{0.4f, 0.0f, 0.0f, -28.0f, 1.0f, 1.0f});
        swim->add_keyframe("RightUpperArm", Keyframe{0.8f, 0.0f, 0.0f, 28.0f, 1.0f, 1.0f});

        swim->add_keyframe("LeftUpperLeg", Keyframe{0.0f, 0.0f, 0.0f, 16.0f, 1.0f, 1.0f});
        swim->add_keyframe("LeftUpperLeg", Keyframe{0.4f, 0.0f, 0.0f, -16.0f, 1.0f, 1.0f});
        swim->add_keyframe("LeftUpperLeg", Keyframe{0.8f, 0.0f, 0.0f, 16.0f, 1.0f, 1.0f});

        swim->add_keyframe("RightUpperLeg", Keyframe{0.0f, 0.0f, 0.0f, -16.0f, 1.0f, 1.0f});
        swim->add_keyframe("RightUpperLeg", Keyframe{0.4f, 0.0f, 0.0f, 16.0f, 1.0f, 1.0f});
        swim->add_keyframe("RightUpperLeg", Keyframe{0.8f, 0.0f, 0.0f, -16.0f, 1.0f, 1.0f});
    }
    create_clip("attack", 0.45f, false);
    create_clip("death", 1.0f, false);
}

} // namespace living_canvas::animation

