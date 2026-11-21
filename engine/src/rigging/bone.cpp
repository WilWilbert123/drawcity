#include "living_canvas/rigging/bone.hpp"

#include <algorithm>
#include <sstream>

namespace living_canvas::rigging {

Bone::Bone(int64_t id, const std::string& name, int64_t parent_bone_id, int64_t start_joint_id, int64_t end_joint_id)
    : id_(id),
      name_(name),
      parent_bone_id_(parent_bone_id),
      start_joint_id_(start_joint_id),
      end_joint_id_(end_joint_id) {}

void Bone::bind_stroke(int64_t stroke_id) {
    if (std::find(bound_stroke_ids_.begin(), bound_stroke_ids_.end(), stroke_id) == bound_stroke_ids_.end()) {
        bound_stroke_ids_.push_back(stroke_id);
    }
}

void Bone::clear_bound_strokes() {
    bound_stroke_ids_.clear();
}

std::string Bone::to_json() const {
    std::ostringstream out;
    out << "{"
        << "\"id\":" << id_ << ","
        << "\"name\":\"" << name_ << "\","
        << "\"parent_bone_id\":" << parent_bone_id_ << ","
        << "\"start_joint_id\":" << start_joint_id_ << ","
        << "\"end_joint_id\":" << end_joint_id_ << ","
        << "\"local_rotation_deg\":" << local_rotation_deg_ << ","
        << "\"length\":" << length_ << ","
        << "\"bound_part_id\":" << bound_part_id_ << ","
        << "\"bound_stroke_ids\":[";
    for (std::size_t i = 0; i < bound_stroke_ids_.size(); ++i) {
        if (i > 0) {
            out << ",";
        }
        out << bound_stroke_ids_[i];
    }
    out << "]}";
    return out.str();
}

} // namespace living_canvas::rigging

