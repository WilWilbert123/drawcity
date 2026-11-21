#ifndef LIVING_CANVAS_RIGGING_JOINT_HPP
#define LIVING_CANVAS_RIGGING_JOINT_HPP

#include <cstdint>
#include <string>

namespace living_canvas::rigging {

class Joint {
public:
    Joint() = default;
    Joint(int64_t id, const std::string& name, float x, float y);

    int64_t id() const noexcept { return id_; }
    const std::string& name() const noexcept { return name_; }

    float x() const noexcept { return x_; }
    float y() const noexcept { return y_; }
    void set_position(float x, float y) noexcept;

    float rest_x() const noexcept { return rest_x_; }
    float rest_y() const noexcept { return rest_y_; }
    void reset_to_rest() noexcept;

    float min_angle_deg() const noexcept { return min_angle_deg_; }
    float max_angle_deg() const noexcept { return max_angle_deg_; }
    void set_angle_limits(float min_deg, float max_deg) noexcept;

    std::string to_json() const;

private:
    int64_t id_ = 0;
    std::string name_ = "Joint";
    float x_ = 0.0f;
    float y_ = 0.0f;
    float rest_x_ = 0.0f;
    float rest_y_ = 0.0f;
    float min_angle_deg_ = -180.0f;
    float max_angle_deg_ = 180.0f;
};

} // namespace living_canvas::rigging

#endif // LIVING_CANVAS_RIGGING_JOINT_HPP

