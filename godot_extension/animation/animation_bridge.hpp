#ifndef LIVING_CANVAS_GDEXT_ANIMATION_BRIDGE_HPP
#define LIVING_CANVAS_GDEXT_ANIMATION_BRIDGE_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string.hpp>

#include "living_canvas/animation/animation_engine.hpp"
#include "living_canvas/rigging/rig_manager.hpp"

namespace living_canvas::gdext {

class AnimationBridge : public godot::RefCounted {
    GDCLASS(AnimationBridge, godot::RefCounted)

public:
    AnimationBridge() = default;
    ~AnimationBridge() override = default;

    int64_t create_rig(int64_t object_id, const godot::String& rig_type);
    int64_t create_auto_stickman_rig(int64_t object_id, double min_x, double min_y, double max_x, double max_y);
    int64_t add_joint(int64_t rig_id, const godot::String& name, double x, double y);
    int64_t add_bone(int64_t rig_id, const godot::String& name, int64_t parent_bone_id, int64_t start_joint_id, int64_t end_joint_id);
    bool move_joint(int64_t rig_id, int64_t joint_id, double x, double y);
    bool rotate_bone(int64_t rig_id, int64_t bone_id, double angle_deg);
    bool solve_two_bone_ik(int64_t rig_id, int64_t upper_bone_id, int64_t lower_bone_id, double target_x, double target_y);
    godot::String serialize_rig_json(int64_t rig_id) const;

    void play_clip(const godot::String& clip_name);
    void pause();
    void stop();
    void set_speed(double speed);
    void step_animation(double delta_sec, int64_t active_rig_id);
    double get_current_time() const;
    godot::String get_active_clip_name() const;
    godot::String get_clip_json(const godot::String& clip_name) const;

protected:
    static void _bind_methods();

private:
    living_canvas::rigging::RigManager rig_manager_;
    living_canvas::animation::AnimationEngine anim_engine_;
};

} // namespace living_canvas::gdext

#endif // LIVING_CANVAS_GDEXT_ANIMATION_BRIDGE_HPP

