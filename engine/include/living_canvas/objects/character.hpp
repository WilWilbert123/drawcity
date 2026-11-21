#ifndef LIVING_CANVAS_OBJECTS_CHARACTER_HPP
#define LIVING_CANVAS_OBJECTS_CHARACTER_HPP

#include "living_canvas/objects/living_object.hpp"

namespace living_canvas::objects {

class CharacterObject : public LivingObject {
public:
    CharacterObject() = default;
    CharacterObject(int64_t id, const std::string& name);

    int64_t held_object_id() const noexcept { return held_object_id_; }
    void set_held_object_id(int64_t object_id) noexcept { held_object_id_ = object_id; }

    int64_t mounted_vehicle_id() const noexcept { return mounted_vehicle_id_; }
    void set_mounted_vehicle_id(int64_t vehicle_id) noexcept { mounted_vehicle_id_ = vehicle_id; }
    bool is_driving() const noexcept { return mounted_vehicle_id_ > 0; }

private:
    int64_t held_object_id_ = 0;
    int64_t mounted_vehicle_id_ = 0;
};

} // namespace living_canvas::objects

#endif // LIVING_CANVAS_OBJECTS_CHARACTER_HPP

