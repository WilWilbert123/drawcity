#include "physics/physics_bridge.hpp"

#include <godot_cpp/core/class_db.hpp>

namespace living_canvas::gdext {

void PhysicsBridge::set_environment(double gravity, double ground_y, double water_y, double wind_vx) {
    physics_.set_gravity(static_cast<float>(gravity));
    physics_.set_ground_y(static_cast<float>(ground_y));
    physics_.set_water_surface_y(static_cast<float>(water_y));
    physics_.set_wind_velocity(static_cast<float>(wind_vx));
    world_.environment().ground_level_y = static_cast<float>(ground_y);
    world_.environment().water_level_y = static_cast<float>(water_y);
    world_.environment().wind_speed_px_s = static_cast<float>(wind_vx);
}

void PhysicsBridge::step_character(int64_t object_id, double move_x, bool jump, double dt) {
    auto& body = physics_.ensure_body(object_id);
    living_canvas::physics::MovementModel::step_ground_character(
        body, static_cast<float>(move_x), jump, physics_.gravity(), physics_.ground_y(), static_cast<float>(dt));
}

void PhysicsBridge::step_car(int64_t object_id, double throttle, double brake, double steer, double wheel_radius, double dt) {
    auto& body = physics_.ensure_body(object_id);
    living_canvas::physics::MovementModel::step_wheeled_vehicle(
        body, static_cast<float>(throttle), static_cast<float>(brake), static_cast<float>(steer),
        static_cast<float>(wheel_radius), physics_.gravity(), physics_.ground_y(), static_cast<float>(dt));
}

void PhysicsBridge::step_airplane(int64_t object_id, double throttle, double pitch_input, double dt) {
    auto& body = physics_.ensure_body(object_id);
    living_canvas::physics::MovementModel::step_aircraft(
        body, static_cast<float>(throttle), static_cast<float>(pitch_input),
        180.0f, 1.35f, physics_.gravity(), physics_.ground_y(), static_cast<float>(dt));
}

void PhysicsBridge::step_fish(int64_t object_id, double swim_x, double swim_y, double dt) {
    auto& body = physics_.ensure_body(object_id);
    living_canvas::physics::MovementModel::step_swimming_fish(
        body, static_cast<float>(swim_x), static_cast<float>(swim_y),
        physics_.water_surface_y(), physics_.gravity(), physics_.ground_y(), static_cast<float>(dt));
}

void PhysicsBridge::step_bird(int64_t object_id, double move_x, bool flap, double dt) {
    auto& body = physics_.ensure_body(object_id);
    living_canvas::physics::MovementModel::step_flying_bird(
        body, static_cast<float>(move_x), flap,
        physics_.wind_velocity(), physics_.gravity(), physics_.ground_y(), static_cast<float>(dt));
}

int64_t PhysicsBridge::mount_vehicle(int64_t character_id, int64_t vehicle_id, double seat_offset_x, double seat_offset_y) {
    return interactions_.mount_vehicle(character_id, vehicle_id, static_cast<float>(seat_offset_x), static_cast<float>(seat_offset_y));
}

bool PhysicsBridge::exit_vehicle(int64_t character_id, int64_t vehicle_id) {
    return interactions_.exit_vehicle(character_id, vehicle_id);
}

int64_t PhysicsBridge::attach_held_object(int64_t character_id, int64_t object_id, double offset_x, double offset_y) {
    return interactions_.attach_held_object(character_id, object_id, static_cast<float>(offset_x), static_cast<float>(offset_y));
}

void PhysicsBridge::step_world(double dt) {
    simulator_.set_running(true);
    simulator_.step(static_cast<float>(dt), world_, physics_, interactions_);
}

void PhysicsBridge::reset_bodies() {
    for (int64_t id : {1, 2, 10, 11}) {
        auto& b = physics_.ensure_body(id);
        b.x = 0.0f;
        b.y = 0.0f;
        b.vx = 0.0f;
        b.vy = 0.0f;
        b.rotation_rad = 0.0f;
        b.wheel_rotation_rad = 0.0f;
    }
    interactions_.clear_relationships();
    simulator_.reset();
}

godot::Dictionary PhysicsBridge::get_body_state(int64_t object_id) {
    const auto& body = physics_.ensure_body(object_id);
    godot::Dictionary d;
    d["x"] = body.x;
    d["y"] = body.y;
    d["vx"] = body.vx;
    d["vy"] = body.vy;
    d["rotation_rad"] = body.rotation_rad;
    d["wheel_rotation_rad"] = body.wheel_rotation_rad;
    d["oxygen_remaining_sec"] = body.oxygen_remaining_sec;
    d["on_ground"] = body.on_ground;
    d["in_water"] = body.in_water;
    d["stalled"] = body.stalled;
    d["alive"] = body.alive;
    return d;
}

void PhysicsBridge::_bind_methods() {
    using namespace godot;
    ClassDB::bind_method(D_METHOD("set_environment", "gravity", "ground_y", "water_y", "wind_vx"), &PhysicsBridge::set_environment);
    ClassDB::bind_method(D_METHOD("step_character", "object_id", "move_x", "jump", "dt"), &PhysicsBridge::step_character);
    ClassDB::bind_method(D_METHOD("step_car", "object_id", "throttle", "brake", "steer", "wheel_radius", "dt"), &PhysicsBridge::step_car);
    ClassDB::bind_method(D_METHOD("step_airplane", "object_id", "throttle", "pitch_input", "dt"), &PhysicsBridge::step_airplane);
    ClassDB::bind_method(D_METHOD("step_fish", "object_id", "swim_x", "swim_y", "dt"), &PhysicsBridge::step_fish);
    ClassDB::bind_method(D_METHOD("step_bird", "object_id", "move_x", "flap", "dt"), &PhysicsBridge::step_bird);
    ClassDB::bind_method(D_METHOD("mount_vehicle", "character_id", "vehicle_id", "seat_offset_x", "seat_offset_y"), &PhysicsBridge::mount_vehicle);
    ClassDB::bind_method(D_METHOD("exit_vehicle", "character_id", "vehicle_id"), &PhysicsBridge::exit_vehicle);
    ClassDB::bind_method(D_METHOD("attach_held_object", "character_id", "object_id", "offset_x", "offset_y"), &PhysicsBridge::attach_held_object);
    ClassDB::bind_method(D_METHOD("step_world", "dt"), &PhysicsBridge::step_world);
    ClassDB::bind_method(D_METHOD("reset_bodies"), &PhysicsBridge::reset_bodies);
    ClassDB::bind_method(D_METHOD("get_body_state", "object_id"), &PhysicsBridge::get_body_state);
}

} // namespace living_canvas::gdext

