#ifndef LIVING_CANVAS_PHYSICS_MOVEMENT_MODEL_HPP
#define LIVING_CANVAS_PHYSICS_MOVEMENT_MODEL_HPP

#include <cstdint>

namespace living_canvas::physics {

enum class LocomotionDomain : int32_t {
    Ground = 0,
    Air = 1,
    Water = 2,
    VehicleWheel = 3,
    Aircraft = 4
};

struct KinematicsState {
    float x = 0.0f;
    float y = 0.0f;
    float vx = 0.0f;
    float vy = 0.0f;
    float rotation_rad = 0.0f;
    float angular_velocity_rad = 0.0f;
    float wheel_rotation_rad = 0.0f;
    float oxygen_remaining_sec = 12.0f;
    bool on_ground = false;
    bool in_water = false;
    bool stalled = false;
    bool alive = true;
};

class MovementModel {
public:
    static void step_ground_character(KinematicsState& state, float move_input_x, bool jump_requested,
                                      float gravity, float ground_y, float dt);
    static void step_wheeled_vehicle(KinematicsState& state, float throttle, float brake, float steer,
                                     float wheel_radius, float gravity, float ground_y, float dt);
    static void step_aircraft(KinematicsState& state, float throttle, float pitch_input,
                              float stall_speed, float lift_coeff, float gravity, float ground_y, float dt);
    static void step_swimming_fish(KinematicsState& state, float swim_x, float swim_y,
                                   float water_surface_y, float gravity, float ground_y, float dt);
    static void step_flying_bird(KinematicsState& state, float move_x, bool flap,
                                 float wind_vx, float gravity, float ground_y, float dt);
};

} // namespace living_canvas::physics

#endif // LIVING_CANVAS_PHYSICS_MOVEMENT_MODEL_HPP

