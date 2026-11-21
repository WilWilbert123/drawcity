#include "living_canvas/storage/project_repository.hpp"
#include "living_canvas/platform/platform_services.hpp"

#include <sqlite3.h>

namespace living_canvas::storage {

ProjectRepository::ProjectRepository(DatabaseManager& db_manager) : db_(db_manager) {}

int64_t ProjectRepository::create_project(int64_t user_id, const std::string& title, const std::string& description,
                                          int32_t canvas_width, int32_t canvas_height) {
    sqlite3* handle = db_.raw_handle();
    if (!handle) {
        return 0;
    }
    const int64_t now = living_canvas::platform::PlatformServices::current_timestamp_ms();
    sqlite3_stmt* stmt = nullptr;
    const char* sql =
        "INSERT INTO projects(user_id, title, description, created_at_ms, updated_at_ms) "
        "VALUES(?1, ?2, ?3, ?4, ?5);";
    if (sqlite3_prepare_v2(handle, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return 0;
    }
    sqlite3_bind_int64(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 4, now);
    sqlite3_bind_int64(stmt, 5, now);

    int64_t project_id = 0;
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        project_id = sqlite3_last_insert_rowid(handle);
    }
    sqlite3_finalize(stmt);

    if (project_id > 0) {
        sqlite3_stmt* c_stmt = nullptr;
        const char* c_sql = "INSERT INTO canvases(project_id, width, height) VALUES(?1, ?2, ?3);";
        if (sqlite3_prepare_v2(handle, c_sql, -1, &c_stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int64(c_stmt, 1, project_id);
            sqlite3_bind_int(c_stmt, 2, canvas_width);
            sqlite3_bind_int(c_stmt, 3, canvas_height);
            if (sqlite3_step(c_stmt) == SQLITE_DONE) {
                const int64_t canvas_id = sqlite3_last_insert_rowid(handle);
                sqlite3_stmt* l_stmt = nullptr;
                const char* l_sql = "INSERT INTO drawing_layers(canvas_id, name, z_index) VALUES(?1, 'Layer 1', 0);";
                if (sqlite3_prepare_v2(handle, l_sql, -1, &l_stmt, nullptr) == SQLITE_OK) {
                    sqlite3_bind_int64(l_stmt, 1, canvas_id);
                    sqlite3_step(l_stmt);
                    sqlite3_finalize(l_stmt);
                }
            }
            sqlite3_finalize(c_stmt);
        }
    }
    return project_id;
}

std::vector<ProjectSummary> ProjectRepository::list_projects(int64_t user_id) const {
    std::vector<ProjectSummary> list;
    sqlite3* handle = db_.raw_handle();
    if (!handle) {
        return list;
    }
    sqlite3_stmt* stmt = nullptr;
    const char* sql = (user_id > 0)
        ? "SELECT project_id, user_id, title, description, created_at_ms, updated_at_ms FROM projects WHERE user_id=?1 ORDER BY updated_at_ms DESC;"
        : "SELECT project_id, user_id, title, description, created_at_ms, updated_at_ms FROM projects ORDER BY updated_at_ms DESC;";
    if (sqlite3_prepare_v2(handle, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return list;
    }
    if (user_id > 0) {
        sqlite3_bind_int64(stmt, 1, user_id);
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ProjectSummary p;
        p.project_id = sqlite3_column_int64(stmt, 0);
        p.user_id = sqlite3_column_int64(stmt, 1);
        const auto* t = sqlite3_column_text(stmt, 2);
        const auto* d = sqlite3_column_text(stmt, 3);
        p.title = t ? reinterpret_cast<const char*>(t) : "";
        p.description = d ? reinterpret_cast<const char*>(d) : "";
        p.created_at_ms = sqlite3_column_int64(stmt, 4);
        p.updated_at_ms = sqlite3_column_int64(stmt, 5);
        list.push_back(std::move(p));
    }
    sqlite3_finalize(stmt);
    return list;
}

bool ProjectRepository::delete_project(int64_t project_id) {
    sqlite3* handle = db_.raw_handle();
    if (!handle) {
        return false;
    }
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(handle, "DELETE FROM projects WHERE project_id=?1;", -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    sqlite3_bind_int64(stmt, 1, project_id);
    const bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return ok;
}

int64_t ProjectRepository::ensure_default_layer(int64_t project_id) {
    sqlite3* handle = db_.raw_handle();
    if (!handle) {
        return 0;
    }
    sqlite3_stmt* stmt = nullptr;
    const char* query =
        "SELECT dl.layer_id FROM drawing_layers dl "
        "JOIN canvases c ON dl.canvas_id = c.canvas_id "
        "WHERE c.project_id = ?1 ORDER BY dl.z_index ASC LIMIT 1;";
    int64_t layer_id = 0;
    if (sqlite3_prepare_v2(handle, query, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int64(stmt, 1, project_id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            layer_id = sqlite3_column_int64(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }
    return layer_id;
}

bool ProjectRepository::save_drawing_strokes(int64_t project_id, const living_canvas::drawing::DrawingEngine& drawing_engine) {
    sqlite3* handle = db_.raw_handle();
    if (!handle) {
        return false;
    }
    const int64_t db_layer_id = ensure_default_layer(project_id);
    if (db_layer_id <= 0) {
        return false;
    }

    db_.execute_sql("BEGIN TRANSACTION;");
    {
        sqlite3_stmt* del_stmt = nullptr;
        if (sqlite3_prepare_v2(handle, "DELETE FROM drawing_strokes WHERE layer_id=?1;", -1, &del_stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int64(del_stmt, 1, db_layer_id);
            sqlite3_step(del_stmt);
            sqlite3_finalize(del_stmt);
        }
    }

    const char* ins_sql =
        "INSERT INTO drawing_strokes(layer_id, object_id, part_id, brush_type, color_hex, base_width, stroke_order, visible, locked, points_blob) "
        "VALUES(?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10);";
    sqlite3_stmt* ins_stmt = nullptr;
    if (sqlite3_prepare_v2(handle, ins_sql, -1, &ins_stmt, nullptr) != SQLITE_OK) {
        db_.execute_sql("ROLLBACK;");
        return false;
    }

    for (const auto& stroke : drawing_engine.strokes()) {
        const auto blob = stroke.serialize_points_blob();
        sqlite3_reset(ins_stmt);
        sqlite3_bind_int64(ins_stmt, 1, db_layer_id);
        sqlite3_bind_int64(ins_stmt, 2, stroke.object_id());
        sqlite3_bind_int64(ins_stmt, 3, stroke.part_id());
        sqlite3_bind_int(ins_stmt, 4, static_cast<int32_t>(stroke.brush_type()));
        sqlite3_bind_text(ins_stmt, 5, stroke.color_hex().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(ins_stmt, 6, stroke.base_width());
        sqlite3_bind_int(ins_stmt, 7, stroke.stroke_order());
        sqlite3_bind_int(ins_stmt, 8, stroke.is_visible() ? 1 : 0);
        sqlite3_bind_int(ins_stmt, 9, stroke.is_locked() ? 1 : 0);
        sqlite3_bind_blob(ins_stmt, 10, blob.data(), static_cast<int>(blob.size()), SQLITE_TRANSIENT);
        if (sqlite3_step(ins_stmt) != SQLITE_DONE) {
            sqlite3_finalize(ins_stmt);
            db_.execute_sql("ROLLBACK;");
            return false;
        }
    }
    sqlite3_finalize(ins_stmt);
    return db_.execute_sql("COMMIT;");
}

bool ProjectRepository::load_drawing_strokes(int64_t project_id, living_canvas::drawing::DrawingEngine& drawing_engine) {
    sqlite3* handle = db_.raw_handle();
    if (!handle) {
        return false;
    }
    const int64_t db_layer_id = ensure_default_layer(project_id);
    if (db_layer_id <= 0) {
        return false;
    }

    const char* sel_sql =
        "SELECT stroke_id, COALESCE(object_id, 0), COALESCE(part_id, 0), brush_type, color_hex, base_width, stroke_order, visible, locked, points_blob "
        "FROM drawing_strokes WHERE layer_id=?1 ORDER BY stroke_order ASC, stroke_id ASC;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(handle, sel_sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    sqlite3_bind_int64(stmt, 1, db_layer_id);

    std::vector<living_canvas::drawing::Stroke> loaded;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const int64_t sid = sqlite3_column_int64(stmt, 0);
        const int64_t obj_id = sqlite3_column_int64(stmt, 1);
        const int64_t prt_id = sqlite3_column_int64(stmt, 2);
        const auto btype = static_cast<living_canvas::drawing::BrushType>(sqlite3_column_int(stmt, 3));
        const auto* ctext = sqlite3_column_text(stmt, 4);
        const std::string color = ctext ? reinterpret_cast<const char*>(ctext) : "#F9FAFB";
        const float width = static_cast<float>(sqlite3_column_double(stmt, 5));
        const int32_t order = sqlite3_column_int(stmt, 6);
        const bool vis = sqlite3_column_int(stmt, 7) != 0;
        const bool lck = sqlite3_column_int(stmt, 8) != 0;

        living_canvas::drawing::Stroke s(sid, 1, btype, color, width);
        s.set_object_id(obj_id);
        s.set_part_id(prt_id);
        s.set_stroke_order(order);
        s.set_visible(vis);
        s.set_locked(lck);

        const void* blob_ptr = sqlite3_column_blob(stmt, 9);
        const int blob_bytes = sqlite3_column_bytes(stmt, 9);
        if (blob_ptr && blob_bytes > 0) {
            s.deserialize_points_blob(static_cast<const uint8_t*>(blob_ptr), static_cast<std::size_t>(blob_bytes));
            s.apply_smoothing(0.35f);
        }
        loaded.push_back(std::move(s));
    }
    sqlite3_finalize(stmt);
    drawing_engine.set_strokes(std::move(loaded));
    return true;
}

} // namespace living_canvas::storage

