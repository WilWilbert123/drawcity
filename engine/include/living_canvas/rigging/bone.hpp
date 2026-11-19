#ifndef LIVING_CANVAS_RIGGING_BONE_HPP
#define LIVING_CANVAS_RIGGING_BONE_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace living_canvas::rigging {

class Bone {
public:
    Bone() = default;
    Bone(int64_t id, const std::string& name, int64_t parent_bone_id, int64_t start_joint_id, int64_t end_joint_id);

    int64_t id() const noexcept { return id_; }
    const std::string& name() const noexcept { return name_; }
    int64_t parent_bone_id() const noexcept { return parent_bone_id_; }
    int64_t start_joint_id() const noexcept { return start_joint_id_; }
    int64_t end_joint_id() const noexcept { return end_joint_id_; }

    float local_rotation_deg() const noexcept { return local_rotation_deg_; }
    void set_local_rotation_deg(float deg) noexcept { local_rotation_deg_ = deg; }

    float length() const noexcept { return length_; }
    void set_length(float length) noexcept { length_ = length; }

    int64_t bound_part_id() const noexcept { return bound_part_id_; }
    void set_bound_part_id(int64_t part_id) noexcept { bound_part_id_ = part_id; }

    const std::vector<int64_t>& bound_stroke_ids() const noexcept { return bound_stroke_ids_; }
    void bind_stroke(int64_t stroke_id);
    void clear_bound_strokes();

    std::string to_json() const;

private:
    int64_t id_ = 0;
    std::string name_ = "Bone";
    int64_t parent_bone_id_ = 0;
    int64_t start_joint_id_ = 0;
    int64_t end_joint_id_ = 0;
    float local_rotation_deg_ = 0.0f;
    float length_ = 30.0f;
    int64_t bound_part_id_ = 0;
    std::vector<int64_t> bound_stroke_ids_;
};

} // namespace living_canvas::rigging

#endif // LIVING_CANVAS_RIGGING_BONE_HPP

