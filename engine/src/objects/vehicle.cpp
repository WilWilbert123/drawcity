#include "living_canvas/objects/vehicle.hpp"

namespace living_canvas::objects {

VehicleObject::VehicleObject(int64_t id, const std::string& name, const std::string& vehicle_type)
    : LivingObject(id, name, vehicle_type), vehicle_type_(vehicle_type) {}

} // namespace living_canvas::objects

