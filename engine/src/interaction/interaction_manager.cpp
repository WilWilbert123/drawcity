#include "living_canvas/interaction/interaction_manager.hpp"

#include <algorithm>
#include <sstream>

namespace living_canvas::interaction {

int64_t InteractionManager::mount_vehicle(int64_t character_id, int64_t vehicle_id, float seat_offset_x, float seat_offset_y) {
    exit_vehicle(character_id, vehicle_id);
    return create_relationship(character_id, vehicle_id, RelationshipType::CharacterDrivesVehicle, seat_offset_x, seat_offset_y);
}

bool InteractionManager::exit_vehicle(int64_t character_id, int64_t vehicle_id) {
    auto old_size = relationships_.size();
    relationships_.erase(
        std::remove_if(relationships_.begin(), relationships_.end(),
                       [character_id, vehicle_id](const ObjectRelationship& r) {
                           return r.source_object_id == character_id &&
                                  (vehicle_id == 0 || r.target_object_id == vehicle_id) &&
                                  r.type == RelationshipType::CharacterDrivesVehicle;
                       }),
        relationships_.end());
    return relationships_.size() != old_size;
}

int64_t InteractionManager::attach_held_object(int64_t character_id, int64_t object_id, float grip_offset_x, float grip_offset_y) {
    drop_held_object(character_id, object_id);
    return create_relationship(character_id, object_id, RelationshipType::CharacterHoldsObject, grip_offset_x, grip_offset_y);
}

bool InteractionManager::drop_held_object(int64_t character_id, int64_t object_id) {
    auto old_size = relationships_.size();
    relationships_.erase(
        std::remove_if(relationships_.begin(), relationships_.end(),
                       [character_id, object_id](const ObjectRelationship& r) {
                           return r.source_object_id == character_id &&
                                  (object_id == 0 || r.target_object_id == object_id) &&
                                  r.type == RelationshipType::CharacterHoldsObject;
                       }),
        relationships_.end());
    return relationships_.size() != old_size;
}

int64_t InteractionManager::create_relationship(int64_t source_id, int64_t target_id, RelationshipType type,
                                                float offset_x, float offset_y) {
    const int64_t id = next_relationship_id_++;
    relationships_.push_back(ObjectRelationship{id, source_id, target_id, type, offset_x, offset_y, true});
    return id;
}

std::string InteractionManager::serialize_json() const {
    std::ostringstream out;
    out << "[";
    for (std::size_t i = 0; i < relationships_.size(); ++i) {
        if (i > 0) {
            out << ",";
        }
        out << relationships_[i].to_json();
    }
    out << "]";
    return out.str();
}

} // namespace living_canvas::interaction

