#include "living_canvas/objects/character.hpp"

namespace living_canvas::objects {

CharacterObject::CharacterObject(int64_t id, const std::string& name)
    : LivingObject(id, name, "stickman") {}

} // namespace living_canvas::objects

