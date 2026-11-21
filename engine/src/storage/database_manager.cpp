#include "living_canvas/storage/database_manager.hpp"
#include "living_canvas/platform/platform_services.hpp"

#include <sqlite3.h>
#include <sstream>

namespace living_canvas::storage {

namespace {

const char* kFullSchemaSql = R"SQL(
PRAGMA foreign_keys = ON;

CREATE TABLE IF NOT EXISTS schema_migrations (
    version INTEGER PRIMARY KEY,
    applied_at_ms INTEGER NOT NULL,
    description TEXT NOT NULL
);

-- 1. users
CREATE TABLE IF NOT EXISTS users (
    user_id INTEGER PRIMARY KEY AUTOINCREMENT,
    display_name TEXT NOT NULL,
    avatar_key TEXT NOT NULL DEFAULT 'artist_01',
    created_at_ms INTEGER NOT NULL,
    last_active_at_ms INTEGER NOT NULL
);

-- 2. user_settings
CREATE TABLE IF NOT EXISTS user_settings (
    setting_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    setting_key TEXT NOT NULL,
    setting_value TEXT NOT NULL,
    updated_at_ms INTEGER NOT NULL,
    UNIQUE(user_id, setting_key),
    FOREIGN KEY(user_id) REFERENCES users(user_id) ON DELETE CASCADE
);

-- 3. projects
CREATE TABLE IF NOT EXISTS projects (
    project_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    title TEXT NOT NULL,
    description TEXT NOT NULL DEFAULT '',
    thumbnail_rel_path TEXT NOT NULL DEFAULT '',
    project_folder_rel_path TEXT NOT NULL DEFAULT '',
    created_at_ms INTEGER NOT NULL,
    updated_at_ms INTEGER NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(user_id) ON DELETE CASCADE
);

-- 4. canvases
CREATE TABLE IF NOT EXISTS canvases (
    canvas_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    width INTEGER NOT NULL DEFAULT 1920,
    height INTEGER NOT NULL DEFAULT 1080,
    background_color TEXT NOT NULL DEFAULT '#181B24',
    zoom_level REAL NOT NULL DEFAULT 1.0,
    pan_x REAL NOT NULL DEFAULT 0.0,
    pan_y REAL NOT NULL DEFAULT 0.0,
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE
);

-- 5. drawing_layers
CREATE TABLE IF NOT EXISTS drawing_layers (
    layer_id INTEGER PRIMARY KEY AUTOINCREMENT,
    canvas_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    z_index INTEGER NOT NULL DEFAULT 0,
    opacity REAL NOT NULL DEFAULT 1.0,
    visible INTEGER NOT NULL DEFAULT 1,
    locked INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY(canvas_id) REFERENCES canvases(canvas_id) ON DELETE CASCADE
);

-- 6. drawing_strokes
CREATE TABLE IF NOT EXISTS drawing_strokes (
    stroke_id INTEGER PRIMARY KEY AUTOINCREMENT,
    layer_id INTEGER NOT NULL,
    object_id INTEGER DEFAULT NULL,
    part_id INTEGER DEFAULT NULL,
    brush_type INTEGER NOT NULL DEFAULT 0,
    color_hex TEXT NOT NULL DEFAULT '#F9FAFB',
    base_width REAL NOT NULL DEFAULT 4.0,
    stroke_order INTEGER NOT NULL DEFAULT 0,
    visible INTEGER NOT NULL DEFAULT 1,
    locked INTEGER NOT NULL DEFAULT 0,
    points_blob BLOB,
    FOREIGN KEY(layer_id) REFERENCES drawing_layers(layer_id) ON DELETE CASCADE
);

-- 7. drawing_points
CREATE TABLE IF NOT EXISTS drawing_points (
    point_id INTEGER PRIMARY KEY AUTOINCREMENT,
    stroke_id INTEGER NOT NULL,
    point_index INTEGER NOT NULL,
    x REAL NOT NULL,
    y REAL NOT NULL,
    pressure REAL NOT NULL DEFAULT 1.0,
    timestamp_ms INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY(stroke_id) REFERENCES drawing_strokes(stroke_id) ON DELETE CASCADE
);

-- 8. objects
CREATE TABLE IF NOT EXISTS objects (
    object_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    category TEXT NOT NULL DEFAULT 'other',
    classification_confidence REAL NOT NULL DEFAULT 1.0,
    pos_x REAL NOT NULL DEFAULT 0.0,
    pos_y REAL NOT NULL DEFAULT 0.0,
    rotation_rad REAL NOT NULL DEFAULT 0.0,
    scale_x REAL NOT NULL DEFAULT 1.0,
    scale_y REAL NOT NULL DEFAULT 1.0,
    created_at_ms INTEGER NOT NULL,
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE
);

-- 9. object_parts
CREATE TABLE IF NOT EXISTS object_parts (
    part_id INTEGER PRIMARY KEY AUTOINCREMENT,
    object_id INTEGER NOT NULL,
    part_name TEXT NOT NULL,
    pivot_x REAL NOT NULL DEFAULT 0.0,
    pivot_y REAL NOT NULL DEFAULT 0.0,
    z_order INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY(object_id) REFERENCES objects(object_id) ON DELETE CASCADE
);

-- 10. rigs
CREATE TABLE IF NOT EXISTS rigs (
    rig_id INTEGER PRIMARY KEY AUTOINCREMENT,
    object_id INTEGER NOT NULL,
    rig_type TEXT NOT NULL DEFAULT 'stickman',
    created_at_ms INTEGER NOT NULL,
    FOREIGN KEY(object_id) REFERENCES objects(object_id) ON DELETE CASCADE
);

-- 11. bones
CREATE TABLE IF NOT EXISTS bones (
    bone_id INTEGER PRIMARY KEY AUTOINCREMENT,
    rig_id INTEGER NOT NULL,
    parent_bone_id INTEGER DEFAULT NULL,
    name TEXT NOT NULL,
    start_joint_id INTEGER NOT NULL,
    end_joint_id INTEGER NOT NULL,
    length REAL NOT NULL DEFAULT 30.0,
    local_rotation_deg REAL NOT NULL DEFAULT 0.0,
    bound_part_id INTEGER DEFAULT NULL,
    FOREIGN KEY(rig_id) REFERENCES rigs(rig_id) ON DELETE CASCADE
);

-- 12. joints
CREATE TABLE IF NOT EXISTS joints (
    joint_id INTEGER PRIMARY KEY AUTOINCREMENT,
    rig_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    pos_x REAL NOT NULL,
    pos_y REAL NOT NULL,
    rest_x REAL NOT NULL,
    rest_y REAL NOT NULL,
    min_angle_deg REAL NOT NULL DEFAULT -180.0,
    max_angle_deg REAL NOT NULL DEFAULT 180.0,
    FOREIGN KEY(rig_id) REFERENCES rigs(rig_id) ON DELETE CASCADE
);

-- 13. animations
CREATE TABLE IF NOT EXISTS animations (
    animation_id INTEGER PRIMARY KEY AUTOINCREMENT,
    object_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    duration_sec REAL NOT NULL DEFAULT 1.0,
    fps INTEGER NOT NULL DEFAULT 24,
    loop INTEGER NOT NULL DEFAULT 1,
    FOREIGN KEY(object_id) REFERENCES objects(object_id) ON DELETE CASCADE
);

-- 14. animation_tracks
CREATE TABLE IF NOT EXISTS animation_tracks (
    track_id INTEGER PRIMARY KEY AUTOINCREMENT,
    animation_id INTEGER NOT NULL,
    target_bone_name TEXT NOT NULL,
    property_type TEXT NOT NULL DEFAULT 'transform',
    FOREIGN KEY(animation_id) REFERENCES animations(animation_id) ON DELETE CASCADE
);

-- 15. keyframes
CREATE TABLE IF NOT EXISTS keyframes (
    keyframe_id INTEGER PRIMARY KEY AUTOINCREMENT,
    track_id INTEGER NOT NULL,
    time_sec REAL NOT NULL,
    offset_x REAL NOT NULL DEFAULT 0.0,
    offset_y REAL NOT NULL DEFAULT 0.0,
    rotation_deg REAL NOT NULL DEFAULT 0.0,
    scale_x REAL NOT NULL DEFAULT 1.0,
    scale_y REAL NOT NULL DEFAULT 1.0,
    easing INTEGER NOT NULL DEFAULT 1,
    FOREIGN KEY(track_id) REFERENCES animation_tracks(track_id) ON DELETE CASCADE
);

-- 16. physics_profiles
CREATE TABLE IF NOT EXISTS physics_profiles (
    profile_id INTEGER PRIMARY KEY AUTOINCREMENT,
    object_id INTEGER NOT NULL,
    body_type TEXT NOT NULL DEFAULT 'dynamic',
    mass REAL NOT NULL DEFAULT 1.0,
    gravity_scale REAL NOT NULL DEFAULT 1.0,
    friction REAL NOT NULL DEFAULT 0.8,
    restitution REAL NOT NULL DEFAULT 0.15,
    buoyancy REAL NOT NULL DEFAULT 0.0,
    FOREIGN KEY(object_id) REFERENCES objects(object_id) ON DELETE CASCADE
);

-- 17. object_relationships
CREATE TABLE IF NOT EXISTS object_relationships (
    relationship_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    source_object_id INTEGER NOT NULL,
    target_object_id INTEGER NOT NULL,
    relationship_type TEXT NOT NULL,
    anchor_offset_x REAL NOT NULL DEFAULT 0.0,
    anchor_offset_y REAL NOT NULL DEFAULT 0.0,
    active INTEGER NOT NULL DEFAULT 1,
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE,
    FOREIGN KEY(source_object_id) REFERENCES objects(object_id) ON DELETE CASCADE,
    FOREIGN KEY(target_object_id) REFERENCES objects(object_id) ON DELETE CASCADE
);

-- 18. worlds
CREATE TABLE IF NOT EXISTS worlds (
    world_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    name TEXT NOT NULL DEFAULT 'Main World',
    gravity_px_s2 REAL NOT NULL DEFAULT 980.0,
    wind_speed_px_s REAL NOT NULL DEFAULT 25.0,
    water_level_y REAL NOT NULL DEFAULT 720.0,
    ground_level_y REAL NOT NULL DEFAULT 880.0,
    time_of_day_hours REAL NOT NULL DEFAULT 12.0,
    weather_type TEXT NOT NULL DEFAULT 'clear',
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE
);

-- 19. world_objects
CREATE TABLE IF NOT EXISTS world_objects (
    world_object_id INTEGER PRIMARY KEY AUTOINCREMENT,
    world_id INTEGER NOT NULL,
    object_id INTEGER NOT NULL,
    spawn_x REAL NOT NULL DEFAULT 0.0,
    spawn_y REAL NOT NULL DEFAULT 0.0,
    active INTEGER NOT NULL DEFAULT 1,
    FOREIGN KEY(world_id) REFERENCES worlds(world_id) ON DELETE CASCADE,
    FOREIGN KEY(object_id) REFERENCES objects(object_id) ON DELETE CASCADE
);

-- 20. assets
CREATE TABLE IF NOT EXISTS assets (
    asset_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    asset_type TEXT NOT NULL,
    relative_path TEXT NOT NULL,
    byte_size INTEGER NOT NULL DEFAULT 0,
    checksum TEXT NOT NULL DEFAULT '',
    created_at_ms INTEGER NOT NULL,
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE
);

-- 21. brush_presets
CREATE TABLE IF NOT EXISTS brush_presets (
    preset_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    brush_type INTEGER NOT NULL,
    default_size REAL NOT NULL,
    min_size REAL NOT NULL,
    max_size REAL NOT NULL,
    opacity REAL NOT NULL,
    smoothing REAL NOT NULL,
    pressure_sensitive INTEGER NOT NULL DEFAULT 1
);

-- 22. project_history
CREATE TABLE IF NOT EXISTS project_history (
    history_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    action_label TEXT NOT NULL,
    snapshot_json TEXT NOT NULL,
    created_at_ms INTEGER NOT NULL,
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE
);

-- 23. export_history
CREATE TABLE IF NOT EXISTS export_history (
    export_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    export_format TEXT NOT NULL,
    output_rel_path TEXT NOT NULL,
    exported_at_ms INTEGER NOT NULL,
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE
);

CREATE INDEX IF NOT EXISTS idx_projects_user ON projects(user_id);
CREATE INDEX IF NOT EXISTS idx_layers_canvas ON drawing_layers(canvas_id);
CREATE INDEX IF NOT EXISTS idx_strokes_layer ON drawing_strokes(layer_id);
CREATE INDEX IF NOT EXISTS idx_strokes_object ON drawing_strokes(object_id);
CREATE INDEX IF NOT EXISTS idx_objects_project ON objects(project_id);
CREATE INDEX IF NOT EXISTS idx_bones_rig ON bones(rig_id);
CREATE INDEX IF NOT EXISTS idx_joints_rig ON joints(rig_id);
CREATE INDEX IF NOT EXISTS idx_animations_object ON animations(object_id);

INSERT OR IGNORE INTO schema_migrations(version, applied_at_ms, description)
VALUES (8, strftime('%s','now') * 1000, 'Complete 23-table LIVING CANVAS offline schema v8');

INSERT OR IGNORE INTO brush_presets(name, brush_type, default_size, min_size, max_size, opacity, smoothing, pressure_sensitive)
VALUES
    ('Pencil', 0, 4.0, 1.0, 32.0, 1.0, 0.35, 1),
    ('Pen', 1, 5.0, 1.0, 48.0, 1.0, 0.50, 1),
    ('Marker', 2, 14.0, 4.0, 64.0, 0.78, 0.45, 0),
    ('Soft Brush', 3, 18.0, 4.0, 64.0, 0.65, 0.60, 1),
    ('Eraser', 4, 20.0, 2.0, 96.0, 1.0, 0.25, 0);
)SQL";

} // namespace

DatabaseManager::DatabaseManager() = default;

DatabaseManager::~DatabaseManager() {
    close();
}

bool DatabaseManager::open(const std::string& db_file_path) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }

    auto slash = db_file_path.find_last_of("/\\");
    if (slash != std::string::npos && slash > 0) {
        living_canvas::platform::PlatformServices::ensure_directory_exists(db_file_path.substr(0, slash));
    }

    if (sqlite3_open(db_file_path.c_str(), &db_) != SQLITE_OK) {
        if (db_) {
            sqlite3_close(db_);
            db_ = nullptr;
        }
        return false;
    }

    db_path_ = db_file_path;
    sqlite3_busy_timeout(db_, 5000);
    char* err = nullptr;
    sqlite3_exec(db_, "PRAGMA journal_mode=WAL; PRAGMA synchronous=NORMAL; PRAGMA foreign_keys=ON;", nullptr, nullptr, &err);
    if (err) {
        sqlite3_free(err);
    }
    return true;
}

void DatabaseManager::close() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

bool DatabaseManager::execute_sql(const std::string& sql, std::string* error_out) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!db_) {
        if (error_out) {
            *error_out = "Database is not open.";
        }
        return false;
    }
    char* err_msg = nullptr;
    const int rc = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        if (error_out && err_msg) {
            *error_out = err_msg;
        }
        if (err_msg) {
            sqlite3_free(err_msg);
        }
        return false;
    }
    return true;
}

bool DatabaseManager::initialize_schema(std::string* error_out) {
    return execute_sql(kFullSchemaSql, error_out);
}

int32_t DatabaseManager::current_schema_version() const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!db_) {
        return 0;
    }
    sqlite3_stmt* stmt = nullptr;
    const char* query = "SELECT COALESCE(MAX(version), 0) FROM schema_migrations;";
    if (sqlite3_prepare_v2(db_, query, -1, &stmt, nullptr) != SQLITE_OK) {
        return 0;
    }
    int32_t version = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        version = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return version;
}

std::vector<std::string> DatabaseManager::list_tables() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<std::string> tables;
    if (!db_) {
        return tables;
    }
    sqlite3_stmt* stmt = nullptr;
    const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%' ORDER BY name;";
    if (sqlite3_prepare_v2(db_, query, -1, &stmt, nullptr) != SQLITE_OK) {
        return tables;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        if (text) {
            tables.emplace_back(reinterpret_cast<const char*>(text));
        }
    }
    sqlite3_finalize(stmt);
    return tables;
}

int64_t DatabaseManager::create_user_profile(const std::string& display_name, const std::string& avatar_key) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!db_) {
        return 0;
    }
    const int64_t now = living_canvas::platform::PlatformServices::current_timestamp_ms();
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "INSERT INTO users(display_name, avatar_key, created_at_ms, last_active_at_ms) VALUES(?1, ?2, ?3, ?4);";
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return 0;
    }
    sqlite3_bind_text(stmt, 1, display_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, avatar_key.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 3, now);
    sqlite3_bind_int64(stmt, 4, now);

    int64_t new_id = 0;
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        new_id = sqlite3_last_insert_rowid(db_);
    }
    sqlite3_finalize(stmt);
    return new_id;
}

std::vector<UserProfileRecord> DatabaseManager::list_user_profiles() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<UserProfileRecord> profiles;
    if (!db_) {
        return profiles;
    }
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT user_id, display_name, avatar_key, created_at_ms FROM users ORDER BY user_id ASC;";
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return profiles;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        UserProfileRecord rec;
        rec.user_id = sqlite3_column_int64(stmt, 0);
        const auto* name = sqlite3_column_text(stmt, 1);
        const auto* avatar = sqlite3_column_text(stmt, 2);
        rec.display_name = name ? reinterpret_cast<const char*>(name) : "";
        rec.avatar_key = avatar ? reinterpret_cast<const char*>(avatar) : "";
        rec.created_at_ms = sqlite3_column_int64(stmt, 3);
        profiles.push_back(std::move(rec));
    }
    sqlite3_finalize(stmt);
    return profiles;
}

bool DatabaseManager::set_user_setting(int64_t user_id, const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!db_) {
        return false;
    }
    const int64_t now = living_canvas::platform::PlatformServices::current_timestamp_ms();
    sqlite3_stmt* stmt = nullptr;
    const char* sql =
        "INSERT INTO user_settings(user_id, setting_key, setting_value, updated_at_ms) "
        "VALUES(?1, ?2, ?3, ?4) "
        "ON CONFLICT(user_id, setting_key) DO UPDATE SET setting_value=excluded.setting_value, updated_at_ms=excluded.updated_at_ms;";
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    sqlite3_bind_int64(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, key.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, value.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 4, now);
    const bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return ok;
}

std::string DatabaseManager::get_user_setting(int64_t user_id, const std::string& key, const std::string& fallback) const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!db_) {
        return fallback;
    }
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT setting_value FROM user_settings WHERE user_id=?1 AND setting_key=?2;";
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return fallback;
    }
    sqlite3_bind_int64(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, key.c_str(), -1, SQLITE_TRANSIENT);
    std::string val = fallback;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const auto* text = sqlite3_column_text(stmt, 0);
        if (text) {
            val = reinterpret_cast<const char*>(text);
        }
    }
    sqlite3_finalize(stmt);
    return val;
}

bool DatabaseManager::create_backup(const std::string& backup_file_path, std::string* error_out) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!db_) {
        if (error_out) {
            *error_out = "Source database not open.";
        }
        return false;
    }
    auto slash = backup_file_path.find_last_of("/\\");
    if (slash != std::string::npos && slash > 0) {
        living_canvas::platform::PlatformServices::ensure_directory_exists(backup_file_path.substr(0, slash));
    }

    sqlite3* dest_db = nullptr;
    if (sqlite3_open(backup_file_path.c_str(), &dest_db) != SQLITE_OK) {
        if (dest_db) {
            sqlite3_close(dest_db);
        }
        if (error_out) {
            *error_out = "Failed to open destination backup database.";
        }
        return false;
    }

    sqlite3_backup* backup = sqlite3_backup_init(dest_db, "main", db_, "main");
    if (!backup) {
        if (error_out) {
            *error_out = sqlite3_errmsg(dest_db);
        }
        sqlite3_close(dest_db);
        return false;
    }

    sqlite3_backup_step(backup, -1);
    const int rc = sqlite3_backup_finish(backup);
    sqlite3_close(dest_db);
    return rc == SQLITE_OK;
}

} // namespace living_canvas::storage

