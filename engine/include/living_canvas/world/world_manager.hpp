#ifndef LIVING_CANVAS_WORLD_WORLD_MANAGER_HPP
#define LIVING_CANVAS_WORLD_WORLD_MANAGER_HPP

#include "living_canvas/objects/living_object.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace living_canvas::world {

struct EnvironmentState {
    float time_of_day_hours = 12.0f; // 0..24
    float wind_speed_px_s = 25.0f;
    float water_level_y = 720.0f;
    float ground_level_y = 880.0f;
    bool raining = false;
};

class WorldManager {
public:
    WorldManager() = default;
    ~WorldManager() = default;

    int64_t register_object(const std::string& name, const std::string& category,
                            const std::vector<int64_t>& stroke_ids, float x, float y);
    bool remove_object(int64_t object_id);
    living_canvas::objects::LivingObject* find_mutable_object(int64_t object_id);
    const living_canvas::objects::LivingObject* find_object(int64_t object_id) const;
    const std::vector<living_canvas::objects::LivingObject>& objects() const noexcept { return objects_; }

    EnvironmentState& environment() noexcept { return env_; }
    const EnvironmentState& environment() const noexcept { return env_; }
    std::string serialize_world_json() const;

private:
    EnvironmentState env_;
    std::vector<living_canvas::objects::LivingObject> objects_;
    int64_t next_object_id_ = 1;
};

} // namespace living_canvas::world

#endif // LIVING_CANVAS_WORLD_WORLD_MANAGER_HPP

