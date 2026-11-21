#ifndef LIVING_CANVAS_PHYSICS_CUSTOM_PHYSICS_HPP
#define LIVING_CANVAS_PHYSICS_CUSTOM_PHYSICS_HPP

#include "living_canvas/physics/movement_model.hpp"

#include <cstdint>
#include <unordered_map>

namespace living_canvas::physics {

class CustomPhysics {
public:
    CustomPhysics() = default;
    ~CustomPhysics() = default;

    void set_gravity(float gravity_px_s2) noexcept { gravity_ = gravity_px_s2; }
    float gravity() const noexcept { return gravity_; }

    void set_ground_y(float ground_y) noexcept { ground_y_ = ground_y; }
    float ground_y() const noexcept { return ground_y_; }

    void set_water_surface_y(float water_y) noexcept { water_surface_y_ = water_y; }
    float water_surface_y() const noexcept { return water_surface_y_; }

    void set_wind_velocity(float wind_vx) noexcept { wind_vx_ = wind_vx; }
    float wind_velocity() const noexcept { return wind_vx_; }

    KinematicsState& ensure_body(int64_t object_id, float initial_x = 0.0f, float initial_y = 0.0f);
    const KinematicsState* find_body(int64_t object_id) const;
    bool compute_aabb_overlap(float ax1, float ay1, float ax2, float ay2,
                              float bx1, float by1, float bx2, float by2) const noexcept;

private:
    float gravity_ = 980.0f;
    float ground_y_ = 880.0f;
    float water_surface_y_ = 720.0f;
    float wind_vx_ = 0.0f;
    std::unordered_map<int64_t, KinematicsState> bodies_;
};

} // namespace living_canvas::physics

#endif // LIVING_CANVAS_PHYSICS_CUSTOM_PHYSICS_HPP

