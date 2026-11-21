#ifndef LIVING_CANVAS_WORLD_WORLD_SIMULATOR_HPP
#define LIVING_CANVAS_WORLD_WORLD_SIMULATOR_HPP

#include "living_canvas/interaction/interaction_manager.hpp"
#include "living_canvas/physics/custom_physics.hpp"
#include "living_canvas/world/world_manager.hpp"

namespace living_canvas::world {

class WorldSimulator {
public:
    WorldSimulator() = default;
    ~WorldSimulator() = default;

    void set_running(bool running) noexcept { running_ = running; }
    bool is_running() const noexcept { return running_; }
    void reset();
    void step(float dt, WorldManager& world, living_canvas::physics::CustomPhysics& physics,
              const living_canvas::interaction::InteractionManager& interactions);

    float elapsed_time_sec() const noexcept { return elapsed_sec_; }

private:
    bool running_ = false;
    float elapsed_sec_ = 0.0f;
};

} // namespace living_canvas::world

#endif // LIVING_CANVAS_WORLD_WORLD_SIMULATOR_HPP

