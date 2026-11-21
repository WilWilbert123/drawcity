#ifndef LIVING_CANVAS_RIGGING_RIG_MANAGER_HPP
#define LIVING_CANVAS_RIGGING_RIG_MANAGER_HPP

#include "living_canvas/drawing/stroke.hpp"
#include "living_canvas/rigging/bone.hpp"
#include "living_canvas/rigging/joint.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace living_canvas::rigging {

struct RigInstance {
    int64_t rig_id = 0;
    int64_t object_id = 0;
    std::string rig_type = "stickman";
    std::vector<Joint> joints;
    std::vector<Bone> bones;
};

class RigManager {
public:
    RigManager() = default;
    ~RigManager() = default;

    int64_t create_rig(int64_t object_id, const std::string& rig_type);
    int64_t add_joint(int64_t rig_id, const std::string& name, float x, float y);
    int64_t add_bone(int64_t rig_id, const std::string& name, int64_t parent_bone_id, int64_t start_joint_id, int64_t end_joint_id);
    bool move_joint(int64_t rig_id, int64_t joint_id, float x, float y);
    bool rotate_bone(int64_t rig_id, int64_t bone_id, float angle_deg);
    bool solve_two_bone_ik(int64_t rig_id, int64_t upper_bone_id, int64_t lower_bone_id, float target_x, float target_y);

    int64_t build_auto_stickman_rig(int64_t object_id, const living_canvas::drawing::BoundingBox& bounds,
                                    const std::vector<living_canvas::drawing::Stroke>& strokes);

    const RigInstance* find_rig(int64_t rig_id) const;
    RigInstance* find_mutable_rig(int64_t rig_id);
    std::string serialize_rig_json(int64_t rig_id) const;

private:
    std::vector<RigInstance> rigs_;
    int64_t next_rig_id_ = 1;
    int64_t next_joint_id_ = 1;
    int64_t next_bone_id_ = 1;
};

} // namespace living_canvas::rigging

#endif // LIVING_CANVAS_RIGGING_RIG_MANAGER_HPP

