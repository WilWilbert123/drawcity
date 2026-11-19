#ifndef LIVING_CANVAS_SERIALIZATION_PROJECT_SERIALIZER_HPP
#define LIVING_CANVAS_SERIALIZATION_PROJECT_SERIALIZER_HPP

#include "living_canvas/animation/animation_engine.hpp"
#include "living_canvas/drawing/drawing_engine.hpp"
#include "living_canvas/interaction/interaction_manager.hpp"
#include "living_canvas/rigging/rig_manager.hpp"
#include "living_canvas/world/world_manager.hpp"

#include <cstdint>
#include <string>

namespace living_canvas::serialization {

struct ProjectBundleMetadata {
    int64_t project_id = 1;
    std::string title = "Untitled Project";
    int64_t owner_user_id = 1;
    int32_t canvas_width = 1920;
    int32_t canvas_height = 1080;
};

class ProjectSerializer {
public:
    static std::string export_project_json(const ProjectBundleMetadata& meta,
                                           const living_canvas::drawing::DrawingEngine& drawing,
                                           const living_canvas::rigging::RigManager& rigging,
                                           const living_canvas::world::WorldManager& world,
                                           const living_canvas::interaction::InteractionManager& interactions);
    static bool save_project_bundle_to_file(const std::string& file_path, const std::string& bundle_json);
};

} // namespace living_canvas::serialization

#endif // LIVING_CANVAS_SERIALIZATION_PROJECT_SERIALIZER_HPP

