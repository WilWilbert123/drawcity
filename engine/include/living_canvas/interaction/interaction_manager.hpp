#ifndef LIVING_CANVAS_INTERACTION_INTERACTION_MANAGER_HPP
#define LIVING_CANVAS_INTERACTION_INTERACTION_MANAGER_HPP

#include "living_canvas/interaction/object_relationship.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace living_canvas::interaction {

class InteractionManager {
public:
    InteractionManager() = default;
    ~InteractionManager() = default;

    int64_t mount_vehicle(int64_t character_id, int64_t vehicle_id, float seat_offset_x = 0.0f, float seat_offset_y = -20.0f);
    bool exit_vehicle(int64_t character_id, int64_t vehicle_id);
    int64_t attach_held_object(int64_t character_id, int64_t object_id, float grip_offset_x = 24.0f, float grip_offset_y = 0.0f);
    bool drop_held_object(int64_t character_id, int64_t object_id);
    int64_t create_relationship(int64_t source_id, int64_t target_id, RelationshipType type,
                                float offset_x = 0.0f, float offset_y = 0.0f);

    const std::vector<ObjectRelationship>& relationships() const noexcept { return relationships_; }
    void clear_relationships() { relationships_.clear(); }
    std::string serialize_json() const;

private:
    std::vector<ObjectRelationship> relationships_;
    int64_t next_relationship_id_ = 1;
};

} // namespace living_canvas::interaction

#endif // LIVING_CANVAS_INTERACTION_INTERACTION_MANAGER_HPP

