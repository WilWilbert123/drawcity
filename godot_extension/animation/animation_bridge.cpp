#include "animation/animation_bridge.hpp"

#include <godot_cpp/core/class_db.hpp>

namespace living_canvas::gdext {

int64_t AnimationBridge::create_rig(int64_t object_id, const godot::String& rig_type) {
    return rig_manager_.create_rig(object_id, rig_type.utf8().get_data());
}

int64_t AnimationBridge::create_auto_stickman_rig(int64_t object_id, double min_x, double min_y, double max_x, double max_y) {
    living_canvas::drawing::BoundingBox box;
    box.min_x = static_cast<float>(min_x);
    box.min_y = static_cast<float>(min_y);
    box.max_x = static_cast<float>(max_x);
    box.max_y = static_cast<float>(max_y);
    box.valid = (max_x > min_x && max_y > min_y);
    return rig_manager_.build_auto_stickman_rig(object_id, box, {});
}

int64_t AnimationBridge::add_joint(int64_t rig_id, const godot::String& name, double x, double y) {
    return rig_manager_.add_joint(rig_id, name.utf8().get_data(), static_cast<float>(x), static_cast<float>(y));
}

int64_t AnimationBridge::add_bone(int64_t rig_id, const godot::String& name, int64_t parent_bone_id, int64_t start_joint_id, int64_t end_joint_id) {
    return rig_manager_.add_bone(rig_id, name.utf8().get_data(), parent_bone_id, start_joint_id, end_joint_id);
}

bool AnimationBridge::move_joint(int64_t rig_id, int64_t joint_id, double x, double y) {
    return rig_manager_.move_joint(rig_id, joint_id, static_cast<float>(x), static_cast<float>(y));
}

bool AnimationBridge::rotate_bone(int64_t rig_id, int64_t bone_id, double angle_deg) {
    return rig_manager_.rotate_bone(rig_id, bone_id, static_cast<float>(angle_deg));
}

bool AnimationBridge::solve_two_bone_ik(int64_t rig_id, int64_t upper_bone_id, int64_t lower_bone_id, double target_x, double target_y) {
    return rig_manager_.solve_two_bone_ik(rig_id, upper_bone_id, lower_bone_id, static_cast<float>(target_x), static_cast<float>(target_y));
}

godot::String AnimationBridge::serialize_rig_json(int64_t rig_id) const {
    return godot::String(rig_manager_.serialize_rig_json(rig_id).c_str());
}

void AnimationBridge::play_clip(const godot::String& clip_name) {
    anim_engine_.play(clip_name.utf8().get_data());
}

void AnimationBridge::pause() {
    anim_engine_.pause();
}

void AnimationBridge::stop() {
    anim_engine_.stop();
}

void AnimationBridge::set_speed(double speed) {
    anim_engine_.set_speed(static_cast<float>(speed));
}

void AnimationBridge::step_animation(double delta_sec, int64_t active_rig_id) {
    auto* rig = rig_manager_.find_mutable_rig(active_rig_id);
    anim_engine_.step(static_cast<float>(delta_sec), rig);
}

double AnimationBridge::get_current_time() const {
    return anim_engine_.current_time_sec();
}

godot::String AnimationBridge::get_active_clip_name() const {
    return godot::String(anim_engine_.active_clip_name().c_str());
}

godot::String AnimationBridge::get_clip_json(const godot::String& clip_name) const {
    const auto* clip = anim_engine_.find_clip(clip_name.utf8().get_data());
    return clip ? godot::String(clip->to_json().c_str()) : godot::String("{}");
}

void AnimationBridge::_bind_methods() {
    using namespace godot;
    ClassDB::bind_method(D_METHOD("create_rig", "object_id", "rig_type"), &AnimationBridge::create_rig);
    ClassDB::bind_method(D_METHOD("create_auto_stickman_rig", "object_id", "min_x", "min_y", "max_x", "max_y"), &AnimationBridge::create_auto_stickman_rig);
    ClassDB::bind_method(D_METHOD("add_joint", "rig_id", "name", "x", "y"), &AnimationBridge::add_joint);
    ClassDB::bind_method(D_METHOD("add_bone", "rig_id", "name", "parent_bone_id", "start_joint_id", "end_joint_id"), &AnimationBridge::add_bone);
    ClassDB::bind_method(D_METHOD("move_joint", "rig_id", "joint_id", "x", "y"), &AnimationBridge::move_joint);
    ClassDB::bind_method(D_METHOD("rotate_bone", "rig_id", "bone_id", "angle_deg"), &AnimationBridge::rotate_bone);
    ClassDB::bind_method(D_METHOD("solve_two_bone_ik", "rig_id", "upper_bone_id", "lower_bone_id", "target_x", "target_y"), &AnimationBridge::solve_two_bone_ik);
    ClassDB::bind_method(D_METHOD("serialize_rig_json", "rig_id"), &AnimationBridge::serialize_rig_json);

    ClassDB::bind_method(D_METHOD("play_clip", "clip_name"), &AnimationBridge::play_clip);
    ClassDB::bind_method(D_METHOD("pause"), &AnimationBridge::pause);
    ClassDB::bind_method(D_METHOD("stop"), &AnimationBridge::stop);
    ClassDB::bind_method(D_METHOD("set_speed", "speed"), &AnimationBridge::set_speed);
    ClassDB::bind_method(D_METHOD("step_animation", "delta_sec", "active_rig_id"), &AnimationBridge::step_animation);
    ClassDB::bind_method(D_METHOD("get_current_time"), &AnimationBridge::get_current_time);
    ClassDB::bind_method(D_METHOD("get_active_clip_name"), &AnimationBridge::get_active_clip_name);
    ClassDB::bind_method(D_METHOD("get_clip_json", "clip_name"), &AnimationBridge::get_clip_json);
}

} // namespace living_canvas::gdext

