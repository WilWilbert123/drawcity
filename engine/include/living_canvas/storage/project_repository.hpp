#ifndef LIVING_CANVAS_STORAGE_PROJECT_REPOSITORY_HPP
#define LIVING_CANVAS_STORAGE_PROJECT_REPOSITORY_HPP

#include "living_canvas/drawing/drawing_engine.hpp"
#include "living_canvas/storage/database_manager.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace living_canvas::storage {

struct ProjectSummary {
    int64_t project_id = 0;
    int64_t user_id = 0;
    std::string title;
    std::string description;
    int64_t created_at_ms = 0;
    int64_t updated_at_ms = 0;
};

class ProjectRepository {
public:
    explicit ProjectRepository(DatabaseManager& db_manager);
    ~ProjectRepository() = default;

    int64_t create_project(int64_t user_id, const std::string& title, const std::string& description = "",
                           int32_t canvas_width = 1920, int32_t canvas_height = 1080);
    std::vector<ProjectSummary> list_projects(int64_t user_id = 0) const;
    bool delete_project(int64_t project_id);

    bool save_drawing_strokes(int64_t project_id, const living_canvas::drawing::DrawingEngine& drawing_engine);
    bool load_drawing_strokes(int64_t project_id, living_canvas::drawing::DrawingEngine& drawing_engine);

private:
    int64_t ensure_default_layer(int64_t project_id);
    DatabaseManager& db_;
};

} // namespace living_canvas::storage

#endif // LIVING_CANVAS_STORAGE_PROJECT_REPOSITORY_HPP

