#include "living_canvas/rigging/rig_manager.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>

namespace living_canvas::rigging {

namespace {

Joint* find_joint(RigInstance& rig, int64_t joint_id) {
    for (auto& j : rig.joints) {
        if (j.id() == joint_id) {
            return &j;
        }
    }
    return nullptr;
}

const Joint* find_joint_const(const RigInstance& rig, int64_t joint_id) {
    for (const auto& j : rig.joints) {
        if (j.id() == joint_id) {
            return &j;
        }
    }
    return nullptr;
}

Bone* find_bone(RigInstance& rig, int64_t bone_id) {
    for (auto& b : rig.bones) {
        if (b.id() == bone_id) {
            return &b;
        }
    }
    return nullptr;
}

} // namespace

int64_t RigManager::create_rig(int64_t object_id, const std::string& rig_type) {
    const int64_t id = next_rig_id_++;
    rigs_.push_back(RigInstance{id, object_id, rig_type, {}, {}});
    return id;
}

int64_t RigManager::add_joint(int64_t rig_id, const std::string& name, float x, float y) {
    auto* rig = find_mutable_rig(rig_id);
    if (!rig) {
        return 0;
    }
    const int64_t id = next_joint_id_++;
    rig->joints.emplace_back(id, name, x, y);
    return id;
}

int64_t RigManager::add_bone(int64_t rig_id, const std::string& name, int64_t parent_bone_id, int64_t start_joint_id, int64_t end_joint_id) {
    auto* rig = find_mutable_rig(rig_id);
    if (!rig) {
        return 0;
    }
    const int64_t id = next_bone_id_++;
    Bone bone(id, name, parent_bone_id, start_joint_id, end_joint_id);
    const auto* start_j = find_joint_const(*rig, start_joint_id);
    const auto* end_j = find_joint_const(*rig, end_joint_id);
    if (start_j && end_j) {
        const float dx = end_j->x() - start_j->x();
        const float dy = end_j->y() - start_j->y();
        bone.set_length(std::sqrt(dx * dx + dy * dy));
    }
    rig->bones.push_back(std::move(bone));
    return id;
}

bool RigManager::move_joint(int64_t rig_id, int64_t joint_id, float x, float y) {
    auto* rig = find_mutable_rig(rig_id);
    if (!rig) {
        return false;
    }
    auto* j = find_joint(*rig, joint_id);
    if (!j) {
        return false;
    }
    j->set_position(x, y);
    return true;
}

bool RigManager::rotate_bone(int64_t rig_id, int64_t bone_id, float angle_deg) {
    auto* rig = find_mutable_rig(rig_id);
    if (!rig) {
        return false;
    }
    auto* bone = find_bone(*rig, bone_id);
    if (!bone) {
        return false;
    }
    bone->set_local_rotation_deg(angle_deg);
    auto* start_j = find_joint(*rig, bone->start_joint_id());
    auto* end_j = find_joint(*rig, bone->end_joint_id());
    if (start_j && end_j) {
        const float rad = angle_deg * 3.1415926535f / 180.0f;
        end_j->set_position(start_j->x() + std::cos(rad) * bone->length(),
                            start_j->y() + std::sin(rad) * bone->length());
    }
    return true;
}

bool RigManager::solve_two_bone_ik(int64_t rig_id, int64_t upper_bone_id, int64_t lower_bone_id, float target_x, float target_y) {
    auto* rig = find_mutable_rig(rig_id);
    if (!rig) {
        return false;
    }
    auto* upper = find_bone(*rig, upper_bone_id);
    auto* lower = find_bone(*rig, lower_bone_id);
    if (!upper || !lower) {
        return false;
    }
    auto* root_j = find_joint(*rig, upper->start_joint_id());
    auto* mid_j = find_joint(*rig, upper->end_joint_id());
    auto* end_j = find_joint(*rig, lower->end_joint_id());
    if (!root_j || !mid_j || !end_j) {
        return false;
    }

    const float l1 = std::max(1.0f, upper->length());
    const float l2 = std::max(1.0f, lower->length());
    const float dx = target_x - root_j->x();
    const float dy = target_y - root_j->y();
    const float dist = std::max(1.0f, std::min(l1 + l2 - 0.01f, std::sqrt(dx * dx + dy * dy)));

    const float base_angle = std::atan2(dy, dx);
    const float cos_angle1 = std::max(-1.0f, std::min(1.0f, (l1 * l1 + dist * dist - l2 * l2) / (2.0f * l1 * dist)));
    const float angle1 = base_angle - std::acos(cos_angle1);

    mid_j->set_position(root_j->x() + std::cos(angle1) * l1, root_j->y() + std::sin(angle1) * l1);
    const float angle2 = std::atan2(target_y - mid_j->y(), target_x - mid_j->x());
    end_j->set_position(mid_j->x() + std::cos(angle2) * l2, mid_j->y() + std::sin(angle2) * l2);

    upper->set_local_rotation_deg(angle1 * 180.0f / 3.1415926535f);
    lower->set_local_rotation_deg(angle2 * 180.0f / 3.1415926535f);
    return true;
}

int64_t RigManager::build_auto_stickman_rig(int64_t object_id, const living_canvas::drawing::BoundingBox& bounds,
                                            const std::vector<living_canvas::drawing::Stroke>& strokes) {
    const int64_t rig_id = create_rig(object_id, "stickman");
    const float cx = bounds.valid ? bounds.center_x() : 500.0f;
    const float top = bounds.valid ? bounds.min_y : 200.0f;
    const float w = bounds.valid ? std::max(60.0f, bounds.width()) : 160.0f;
    const float h = bounds.valid ? std::max(120.0f, bounds.height()) : 300.0f;

    const int64_t j_pelvis = add_joint(rig_id, "Pelvis", cx, top + h * 0.56f);
    const int64_t j_neck = add_joint(rig_id, "Neck", cx, top + h * 0.25f);
    const int64_t j_head = add_joint(rig_id, "HeadTop", cx, top + h * 0.05f);

    const int64_t j_l_elbow = add_joint(rig_id, "LeftElbow", cx - w * 0.25f, top + h * 0.36f);
    const int64_t j_l_hand = add_joint(rig_id, "LeftHand", cx - w * 0.45f, top + h * 0.48f);
    const int64_t j_r_elbow = add_joint(rig_id, "RightElbow", cx + w * 0.25f, top + h * 0.36f);
    const int64_t j_r_hand = add_joint(rig_id, "RightHand", cx + w * 0.45f, top + h * 0.48f);

    const int64_t j_l_knee = add_joint(rig_id, "LeftKnee", cx - w * 0.16f, top + h * 0.76f);
    const int64_t j_l_foot = add_joint(rig_id, "LeftFoot", cx - w * 0.22f, top + h * 0.98f);
    const int64_t j_r_knee = add_joint(rig_id, "RightKnee", cx + w * 0.16f, top + h * 0.76f);
    const int64_t j_r_foot = add_joint(rig_id, "RightFoot", cx + w * 0.22f, top + h * 0.98f);

    const int64_t b_body = add_bone(rig_id, "Body", 0, j_pelvis, j_neck);
    add_bone(rig_id, "Head", b_body, j_neck, j_head);

    const int64_t b_l_upper_arm = add_bone(rig_id, "LeftUpperArm", b_body, j_neck, j_l_elbow);
    add_bone(rig_id, "LeftLowerArm", b_l_upper_arm, j_l_elbow, j_l_hand);

    const int64_t b_r_upper_arm = add_bone(rig_id, "RightUpperArm", b_body, j_neck, j_r_elbow);
    add_bone(rig_id, "RightLowerArm", b_r_upper_arm, j_r_elbow, j_r_hand);

    const int64_t b_l_upper_leg = add_bone(rig_id, "LeftUpperLeg", b_body, j_pelvis, j_l_knee);
    add_bone(rig_id, "LeftLowerLeg", b_l_upper_leg, j_l_knee, j_l_foot);

    const int64_t b_r_upper_leg = add_bone(rig_id, "RightUpperLeg", b_body, j_pelvis, j_r_knee);
    add_bone(rig_id, "RightLowerLeg", b_r_upper_leg, j_r_knee, j_r_foot);

    // Associate each original user stroke with its nearest bone midpoint without altering original stroke data
    auto* rig = find_mutable_rig(rig_id);
    if (rig) {
        for (const auto& s : strokes) {
            const auto sb = s.compute_bounds();
            if (!sb.valid) {
                continue;
            }
            const float sx = sb.center_x();
            const float sy = sb.center_y();
            float best_dist2 = 1e30f;
            Bone* best_bone = nullptr;
            for (auto& bone : rig->bones) {
                const auto* sj = find_joint_const(*rig, bone.start_joint_id());
                const auto* ej = find_joint_const(*rig, bone.end_joint_id());
                if (!sj || !ej) {
                    continue;
                }
                const float mx = (sj->x() + ej->x()) * 0.5f;
                const float my = (sj->y() + ej->y()) * 0.5f;
                const float dx = sx - mx;
                const float dy = sy - my;
                const float d2 = dx * dx + dy * dy;
                if (d2 < best_dist2) {
                    best_dist2 = d2;
                    best_bone = &bone;
                }
            }
            if (best_bone) {
                best_bone->bind_stroke(s.id());
            }
        }
    }

    return rig_id;
}

const RigInstance* RigManager::find_rig(int64_t rig_id) const {
    for (const auto& r : rigs_) {
        if (r.rig_id == rig_id) {
            return &r;
        }
    }
    return nullptr;
}

RigInstance* RigManager::find_mutable_rig(int64_t rig_id) {
    for (auto& r : rigs_) {
        if (r.rig_id == rig_id) {
            return &r;
        }
    }
    return nullptr;
}

std::string RigManager::serialize_rig_json(int64_t rig_id) const {
    const auto* rig = find_rig(rig_id);
    if (!rig) {
        return "{}";
    }
    std::ostringstream out;
    out << "{"
        << "\"rig_id\":" << rig->rig_id << ","
        << "\"object_id\":" << rig->object_id << ","
        << "\"rig_type\":\"" << rig->rig_type << "\","
        << "\"joints\":[";
    for (std::size_t i = 0; i < rig->joints.size(); ++i) {
        if (i > 0) {
            out << ",";
        }
        out << rig->joints[i].to_json();
    }
    out << "],\"bones\":[";
    for (std::size_t i = 0; i < rig->bones.size(); ++i) {
        if (i > 0) {
            out << ",";
        }
        out << rig->bones[i].to_json();
    }
    out << "]}";
    return out.str();
}

} // namespace living_canvas::rigging

