#include "living_canvas/physics/movement_model.hpp"

#include <algorithm>
#include <cmath>

namespace living_canvas::physics {

void MovementModel::step_ground_character(KinematicsState& state, float move_input_x, bool jump_requested,
                                          float gravity, float ground_y, float dt) {
    const float target_vx = move_input_x * 260.0f;
    state.vx = state.vx * 0.8f + target_vx * 0.2f;
    if (jump_requested && state.on_ground) {
        state.vy = -460.0f;
        state.on_ground = false;
    }
    state.vy += gravity * dt;
    state.x += state.vx * dt;
    state.y += state.vy * dt;

    if (state.y >= ground_y) {
        state.y = ground_y;
        state.vy = 0.0f;
        state.on_ground = true;
    } else {
        state.on_ground = false;
    }
}

void MovementModel::step_wheeled_vehicle(KinematicsState& state, float throttle, float brake, float steer,
                                         float wheel_radius, float gravity, float ground_y, float dt) {
    const float accel = throttle * 450.0f;
    state.vx += accel * dt;

    if (brake > 0.01f) {
        const float brake_force = brake * 680.0f * dt;
        if (std::fabs(state.vx) <= brake_force) {
            state.vx = 0.0f;
        } else {
            state.vx -= (state.vx > 0.0f ? brake_force : -brake_force);
        }
    }

    // Rolling drag
    state.vx *= std::pow(0.88f, dt);
    state.vx = std::max(-650.0f, std::min(650.0f, state.vx));

    state.vy += gravity * dt;
    state.x += state.vx * dt;
    state.y += state.vy * dt;

    if (state.y >= ground_y) {
        state.y = ground_y;
        state.vy = 0.0f;
        state.on_ground = true;
    }
    state.rotation_rad = steer * 0.12f;
    const float safe_radius = std::max(8.0f, wheel_radius);
    state.wheel_rotation_rad += (state.vx * dt) / safe_radius;
}

void MovementModel::step_aircraft(KinematicsState& state, float throttle, float pitch_input,
                                  float stall_speed, float lift_coeff, float gravity, float ground_y, float dt) {
    state.rotation_rad = std::max(-0.65f, std::min(0.65f, state.rotation_rad + pitch_input * 1.4f * dt));
    const float thrust = throttle * 520.0f;
    state.vx += std::cos(state.rotation_rad) * thrust * dt;
    state.vx *= std::pow(0.75f, dt);

    const float airspeed = std::fabs(state.vx);
    state.stalled = (airspeed < stall_speed && !state.on_ground);
    const float lift = state.stalled ? (airspeed * 0.8f) : (airspeed * lift_coeff * 2.5f);

    state.vy += (gravity - lift) * dt;
    state.x += state.vx * dt;
    state.y += state.vy * dt;

    if (state.y >= ground_y) {
        state.y = ground_y;
        state.vy = 0.0f;
        state.on_ground = true;
        state.stalled = false;
    } else {
        state.on_ground = false;
    }
}

void MovementModel::step_swimming_fish(KinematicsState& state, float swim_x, float swim_y,
                                       float water_surface_y, float gravity, float ground_y, float dt) {
    state.in_water = (state.y >= water_surface_y);
    if (state.in_water) {
        state.oxygen_remaining_sec = std::min(12.0f, state.oxygen_remaining_sec + dt * 2.5f);
        if (state.alive) {
            state.vx = state.vx * 0.85f + (swim_x * 210.0f) * 0.15f;
            state.vy = state.vy * 0.85f + (swim_y * 160.0f) * 0.15f;
        } else {
            // Float gently toward surface if oxygen reached zero
            state.vx *= 0.9f;
            state.vy = -30.0f;
        }
    } else {
        // Outside water: lose oxygen over time and struggle under gravity
        state.oxygen_remaining_sec = std::max(0.0f, state.oxygen_remaining_sec - dt);
        if (state.oxygen_remaining_sec <= 0.0f) {
            state.alive = false;
        }
        state.vy += gravity * dt;
        if (state.alive && state.on_ground) {
            // Occasional flop when struggling outside water
            state.vy = -160.0f;
        }
    }

    state.x += state.vx * dt;
    state.y += state.vy * dt;
    if (state.y >= ground_y) {
        state.y = ground_y;
        state.vy = 0.0f;
        state.on_ground = true;
    } else {
        state.on_ground = false;
    }
}

void MovementModel::step_flying_bird(KinematicsState& state, float move_x, bool flap,
                                     float wind_vx, float gravity, float ground_y, float dt) {
    state.vx = state.vx * 0.85f + (move_x * 240.0f + wind_vx) * 0.15f;
    if (flap) {
        state.vy = -260.0f;
        state.on_ground = false;
    }
    state.vy += (gravity * 0.55f) * dt;
    state.x += state.vx * dt;
    state.y += state.vy * dt;
    if (state.y >= ground_y) {
        state.y = ground_y;
        state.vy = 0.0f;
        state.on_ground = true;
    } else {
        state.on_ground = false;
    }
}

} // namespace living_canvas::physics

