#include "living_canvas/world/world_manager.hpp"

#include <algorithm>
#include <sstream>

namespace living_canvas::world {

int64_t WorldManager::register_object(const std::string& name, const std::string& category,
                                      const std::vector<int64_t>& stroke_ids, float x, float y) {
    const int64_t id = next_object_id_++;
    living_canvas::objects::LivingObject obj(id, name, category);
    obj.set_position(x, y);
    for (int64_t sid : stroke_ids) {
        obj.add_stroke_id(sid);
    }
    objects_.push_back(std::move(obj));
    return id;
}

bool WorldManager::remove_object(int64_t object_id) {
    auto old_size = objects_.size();
    objects_.erase(
        std::remove_if(objects_.begin(), objects_.end(),
                       [object_id](const living_canvas::objects::LivingObject& o) { return o.id() == object_id; }),
        objects_.end());
    return objects_.size() != old_size;
}

living_canvas::objects::LivingObject* WorldManager::find_mutable_object(int64_t object_id) {
    for (auto& o : objects_) {
        if (o.id() == object_id) {
            return &o;
        }
    }
    return nullptr;
}

const living_canvas::objects::LivingObject* WorldManager::find_object(int64_t object_id) const {
    for (const auto& o : objects_) {
        if (o.id() == object_id) {
            return &o;
        }
    }
    return nullptr;
}

std::string WorldManager::serialize_world_json() const {
    std::ostringstream out;
    out << "{"
        << "\"environment\":{"
        << "\"time_of_day_hours\":" << env_.time_of_day_hours << ","
        << "\"wind_speed_px_s\":" << env_.wind_speed_px_s << ","
        << "\"water_level_y\":" << env_.water_level_y << ","
        << "\"ground_level_y\":" << env_.ground_level_y << ","
        << "\"raining\":" << (env_.raining ? "true" : "false")
        << "},\"objects\":[";
    for (std::size_t i = 0; i < objects_.size(); ++i) {
        if (i > 0) {
            out << ",";
        }
        out << objects_[i].to_json();
    }
    out << "]}";
    return out.str();
}

} // namespace living_canvas::world

