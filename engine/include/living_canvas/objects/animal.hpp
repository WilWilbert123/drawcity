#ifndef LIVING_CANVAS_OBJECTS_ANIMAL_HPP
#define LIVING_CANVAS_OBJECTS_ANIMAL_HPP

#include "living_canvas/objects/living_object.hpp"

namespace living_canvas::objects {

class AnimalObject : public LivingObject {
public:
    AnimalObject() = default;
    AnimalObject(int64_t id, const std::string& name, const std::string& species);

    const std::string& species() const noexcept { return species_; }
    float oxygen_level() const noexcept { return oxygen_level_; }
    void set_oxygen_level(float oxygen) noexcept { oxygen_level_ = oxygen; }
    bool is_aquatic() const noexcept { return species_ == "fish"; }
    bool can_fly() const noexcept { return species_ == "bird" || species_ == "dragon"; }

private:
    std::string species_ = "animal";
    float oxygen_level_ = 12.0f;
};

} // namespace living_canvas::objects

#endif // LIVING_CANVAS_OBJECTS_ANIMAL_HPP

