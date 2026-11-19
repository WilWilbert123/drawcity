#ifndef LIVING_CANVAS_OBJECTS_VEHICLE_HPP
#define LIVING_CANVAS_OBJECTS_VEHICLE_HPP

#include "living_canvas/objects/living_object.hpp"

namespace living_canvas::objects {

class VehicleObject : public LivingObject {
public:
    VehicleObject() = default;
    VehicleObject(int64_t id, const std::string& name, const std::string& vehicle_type);

    const std::string& vehicle_type() const noexcept { return vehicle_type_; }
    int64_t driver_character_id() const noexcept { return driver_character_id_; }
    void set_driver_character_id(int64_t driver_id) noexcept { driver_character_id_ = driver_id; }

    float wheel_radius() const noexcept { return wheel_radius_; }
    void set_wheel_radius(float radius) noexcept { wheel_radius_ = radius; }

    bool is_aircraft() const noexcept { return vehicle_type_ == "airplane"; }

private:
    std::string vehicle_type_ = "car";
    int64_t driver_character_id_ = 0;
    float wheel_radius_ = 26.0f;
};

} // namespace living_canvas::objects

#endif // LIVING_CANVAS_OBJECTS_VEHICLE_HPP

