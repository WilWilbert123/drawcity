#include "living_canvas/physics/custom_physics.hpp"

namespace living_canvas::physics {

KinematicsState& CustomPhysics::ensure_body(int64_t object_id, float initial_x, float initial_y) {
    auto it = bodies_.find(object_id);
    if (it == bodies_.end()) {
        KinematicsState st;
        st.x = initial_x;
        st.y = initial_y;
        auto [inserted, _] = bodies_.emplace(object_id, st);
        return inserted->second;
    }
    return it->second;
}

const KinematicsState* CustomPhysics::find_body(int64_t object_id) const {
    auto it = bodies_.find(object_id);
    if (it == bodies_.end()) {
        return nullptr;
    }
    return &it->second;
}

bool CustomPhysics::compute_aabb_overlap(float ax1, float ay1, float ax2, float ay2,
                                         float bx1, float by1, float bx2, float by2) const noexcept {
    return (ax1 <= bx2 && ax2 >= bx1 && ay1 <= by2 && ay2 >= by1);
}

} // namespace living_canvas::physics

