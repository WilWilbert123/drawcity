#include "living_canvas/objects/animal.hpp"

namespace living_canvas::objects {

AnimalObject::AnimalObject(int64_t id, const std::string& name, const std::string& species)
    : LivingObject(id, name, species), species_(species) {}

} // namespace living_canvas::objects

