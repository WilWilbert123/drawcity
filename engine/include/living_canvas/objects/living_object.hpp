#ifndef LIVING_CANVAS_OBJECTS_LIVING_OBJECT_HPP
#define LIVING_CANVAS_OBJECTS_LIVING_OBJECT_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace living_canvas::objects {

struct ObjectPart {
    int64_t part_id = 0;
    std::string part_name;
    std::vector<int64_t> stroke_ids;
};

class LivingObject {
public:
    LivingObject() = default;
    LivingObject(int64_t id, const std::string& name, const std::string& category);
    virtual ~LivingObject() = default;

    int64_t id() const noexcept { return id_; }
    const std::string& name() const noexcept { return name_; }
    void set_name(const std::string& name) { name_ = name; }

    const std::string& category() const noexcept { return category_; }
    void set_category(const std::string& category) { category_ = category; }

    int64_t rig_id() const noexcept { return rig_id_; }
    void set_rig_id(int64_t rig_id) noexcept { rig_id_ = rig_id; }

    void add_stroke_id(int64_t stroke_id);
    const std::vector<int64_t>& stroke_ids() const noexcept { return stroke_ids_; }

    int64_t add_part(const std::string& part_name, const std::vector<int64_t>& stroke_ids);
    const std::vector<ObjectPart>& parts() const noexcept { return parts_; }

    float x() const noexcept { return x_; }
    float y() const noexcept { return y_; }
    void set_position(float x, float y) noexcept {
        x_ = x;
        y_ = y;
    }

    float rotation_rad() const noexcept { return rotation_rad_; }
    void set_rotation_rad(float rad) noexcept { rotation_rad_ = rad; }

    std::string to_json() const;

protected:
    int64_t id_ = 0;
    std::string name_ = "Drawn Object";
    std::string category_ = "other";
    int64_t rig_id_ = 0;
    float x_ = 0.0f;
    float y_ = 0.0f;
    float rotation_rad_ = 0.0f;
    std::vector<int64_t> stroke_ids_;
    std::vector<ObjectPart> parts_;
    int64_t next_part_id_ = 1;
};

} // namespace living_canvas::objects

#endif // LIVING_CANVAS_OBJECTS_LIVING_OBJECT_HPP

