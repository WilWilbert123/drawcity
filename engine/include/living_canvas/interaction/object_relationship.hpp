#ifndef LIVING_CANVAS_INTERACTION_OBJECT_RELATIONSHIP_HPP
#define LIVING_CANVAS_INTERACTION_OBJECT_RELATIONSHIP_HPP

#include <cstdint>
#include <string>

namespace living_canvas::interaction {

enum class RelationshipType : int32_t {
    None = 0,
    CharacterDrivesVehicle = 1,
    CharacterHoldsObject = 2,
    CharacterStandsOnPlatform = 3,
    BirdLandsOnTree = 4,
    FishSwimsInWater = 5,
    AirplaneFliesInSky = 6,
    ObjectTriggersObject = 7
};

struct ObjectRelationship {
    int64_t relationship_id = 0;
    int64_t source_object_id = 0;
    int64_t target_object_id = 0;
    RelationshipType type = RelationshipType::None;
    float anchor_offset_x = 0.0f;
    float anchor_offset_y = 0.0f;
    bool active = true;

    std::string to_json() const;
    static std::string type_to_string(RelationshipType type);
};

} // namespace living_canvas::interaction

#endif // LIVING_CANVAS_INTERACTION_OBJECT_RELATIONSHIP_HPP

