#include "living_canvas/serialization/project_serializer.hpp"
#include "living_canvas/platform/platform_services.hpp"

#include <sstream>

namespace living_canvas::serialization {

std::string ProjectSerializer::export_project_json(const ProjectBundleMetadata& meta,
                                                   const living_canvas::drawing::DrawingEngine& drawing,
                                                   const living_canvas::rigging::RigManager& rigging,
                                                   const living_canvas::world::WorldManager& world,
                                                   const living_canvas::interaction::InteractionManager& interactions) {
    std::ostringstream out;
    out << "{\n"
        << "  \"format\": \"LIVING_CANVAS_PROJECT_V1\",\n"
        << "  \"preserve_original_artwork\": true,\n"
        << "  \"project_id\": " << meta.project_id << ",\n"
        << "  \"title\": \"" << meta.title << "\",\n"
        << "  \"owner_user_id\": " << meta.owner_user_id << ",\n"
        << "  \"canvas_width\": " << meta.canvas_width << ",\n"
        << "  \"canvas_height\": " << meta.canvas_height << ",\n"
        << "  \"updated_at_ms\": " << living_canvas::platform::PlatformServices::current_timestamp_ms() << ",\n"
        << "  \"drawing\": " << drawing.serialize_canvas_json() << ",\n"
        << "  \"world\": " << world.serialize_world_json() << ",\n"
        << "  \"relationships\": " << interactions.serialize_json() << ",\n"
        << "  \"rigs\": [";
    bool first_rig = true;
    for (const auto& obj : world.objects()) {
        if (obj.rig_id() > 0 && rigging.find_rig(obj.rig_id())) {
            if (!first_rig) {
                out << ",";
            }
            out << rigging.serialize_rig_json(obj.rig_id());
            first_rig = false;
        }
    }
    out << "]\n}";
    return out.str();
}

bool ProjectSerializer::save_project_bundle_to_file(const std::string& file_path, const std::string& bundle_json) {
    return living_canvas::platform::PlatformServices::atomic_write_text_file(file_path, bundle_json);
}

} // namespace living_canvas::serialization

