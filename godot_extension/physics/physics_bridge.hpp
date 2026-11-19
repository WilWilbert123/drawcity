#ifndef LIVING_CANVAS_GDEXT_PHYSICS_BRIDGE_HPP
#define LIVING_CANVAS_GDEXT_PHYSICS_BRIDGE_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/string.hpp>

#include "living_canvas/interaction/interaction_manager.hpp"
#include "living_canvas/physics/custom_physics.hpp"
#include "living_canvas/world/world_manager.hpp"
#include "living_canvas/world/world_simulator.hpp"

namespace living_canvas::gdext {

class PhysicsBridge : public godot::RefCounted {
    GDCLASS(PhysicsBridge, godot::RefCounted)

public:
    PhysicsBridge() = default;
    ~PhysicsBridge() override = default;

    void set_environment(double gravity, double ground_y, double water_y, double wind_vx);
    void step_character(int64_t object_id, double move_x, bool jump, double dt);
    void step_car(int64_t object_id, double throttle, double brake, double steer, double wheel_radius, double dt);
    void step_airplane(int64_t object_id, double throttle, double pitch_input, double dt);
    void step_fish(int64_t object_id, double swim_x, double swim_y, double dt);
    void step_bird(int64_t object_id, double move_x, bool flap, double dt);

    int64_t mount_vehicle(int64_t character_id, int64_t vehicle_id, double seat_offset_x, double seat_offset_y);
    bool exit_vehicle(int64_t character_id, int64_t vehicle_id);
    int64_t attach_held_object(int64_t character_id, int64_t object_id, double offset_x, double offset_y);
    void step_world(double dt);

    godot::Dictionary get_body_state(int64_t object_id);

protected:
    static void _bind_methods();

private:
    living_canvas::physics::CustomPhysics physics_;
    living_canvas::interaction::InteractionManager interactions_;
    living_canvas::world::WorldManager world_;
    living_canvas::world::WorldSimulator simulator_;
};

} // namespace living_canvas::gdext

#endif // LIVING_CANVAS_GDEXT_PHYSICS_BRIDGE_HPP

