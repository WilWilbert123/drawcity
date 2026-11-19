#include "living_canvas/objects/living_object.hpp"

#include <algorithm>
#include <sstream>

namespace living_canvas::objects {

LivingObject::LivingObject(int64_t id, const std::string& name, const std::string& category)
    : id_(id), name_(name), category_(category) {}

void LivingObject::add_stroke_id(int64_t stroke_id) {
    if (std::find(stroke_ids_.begin(), stroke_ids_.end(), stroke_id) == stroke_ids_.end()) {
        stroke_ids_.push_back(stroke_id);
    }
}

int64_t LivingObject::add_part(const std::string& part_name, const std::vector<int64_t>& stroke_ids) {
    const int64_t pid = next_part_id_++;
    parts_.push_back(ObjectPart{pid, part_name, stroke_ids});
    for (int64_t sid : stroke_ids) {
        add_stroke_id(sid);
    }
    return pid;
}

std::string LivingObject::to_json() const {
    std::ostringstream out;
    out << "{"
        << "\"id\":" << id_ << ","
        << "\"name\":\"" << name_ << "\","
        << "\"category\":\"" << category_ << "\","
        << "\"rig_id\":" << rig_id_ << ","
        << "\"x\":" << x_ << ","
        << "\"y\":" << y_ << ","
        << "\"rotation_rad\":" << rotation_rad_ << ","
        << "\"stroke_ids\":[";
    for (std::size_t i = 0; i < stroke_ids_.size(); ++i) {
        if (i > 0) {
            out << ",";
        }
        out << stroke_ids_[i];
    }
    out << "]}";
    return out.str();
}

} // namespace living_canvas::objects

