#include "living_canvas/rigging/joint.hpp"

#include <algorithm>
#include <sstream>

namespace living_canvas::rigging {

Joint::Joint(int64_t id, const std::string& name, float x, float y)
    : id_(id), name_(name), x_(x), y_(y), rest_x_(x), rest_y_(y) {}

void Joint::set_position(float x, float y) noexcept {
    x_ = x;
    y_ = y;
}

void Joint::reset_to_rest() noexcept {
    x_ = rest_x_;
    y_ = rest_y_;
}

void Joint::set_angle_limits(float min_deg, float max_deg) noexcept {
    min_angle_deg_ = std::min(min_deg, max_deg);
    max_angle_deg_ = std::max(min_deg, max_deg);
}

std::string Joint::to_json() const {
    std::ostringstream out;
    out << "{"
        << "\"id\":" << id_ << ","
        << "\"name\":\"" << name_ << "\","
        << "\"x\":" << x_ << ","
        << "\"y\":" << y_ << ","
        << "\"rest_x\":" << rest_x_ << ","
        << "\"rest_y\":" << rest_y_ << ","
        << "\"min_angle_deg\":" << min_angle_deg_ << ","
        << "\"max_angle_deg\":" << max_angle_deg_
        << "}";
    return out.str();
}

} // namespace living_canvas::rigging

