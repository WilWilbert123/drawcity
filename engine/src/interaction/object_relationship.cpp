#include "living_canvas/interaction/object_relationship.hpp"

#include <sstream>

namespace living_canvas::interaction {

std::string ObjectRelationship::type_to_string(RelationshipType type) {
    switch (type) {
        case RelationshipType::CharacterDrivesVehicle: return "drives_vehicle";
        case RelationshipType::CharacterHoldsObject: return "holds_object";
        case RelationshipType::CharacterStandsOnPlatform: return "stands_on_platform";
        case RelationshipType::BirdLandsOnTree: return "lands_on_tree";
        case RelationshipType::FishSwimsInWater: return "swims_in_water";
        case RelationshipType::AirplaneFliesInSky: return "flies_in_sky";
        case RelationshipType::ObjectTriggersObject: return "triggers_object";
        default: return "none";
    }
}

std::string ObjectRelationship::to_json() const {
    std::ostringstream out;
    out << "{"
        << "\"id\":" << relationship_id << ","
        << "\"source_object_id\":" << source_object_id << ","
        << "\"target_object_id\":" << target_object_id << ","
        << "\"type\":\"" << type_to_string(type) << "\","
        << "\"anchor_offset_x\":" << anchor_offset_x << ","
        << "\"anchor_offset_y\":" << anchor_offset_y << ","
        << "\"active\":" << (active ? "true" : "false")
        << "}";
    return out.str();
}

} // namespace living_canvas::interaction

