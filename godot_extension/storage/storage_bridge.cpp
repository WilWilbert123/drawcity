#include "storage/storage_bridge.hpp"

#include <godot_cpp/core/class_db.hpp>

namespace living_canvas::gdext {

bool StorageBridge::open_database(const godot::String& absolute_db_path) {
    if (!db_.open(absolute_db_path.utf8().get_data())) {
        return false;
    }
    return db_.initialize_schema();
}

void StorageBridge::close_database() {
    db_.close();
}

bool StorageBridge::is_open() const {
    return db_.is_open();
}

int32_t StorageBridge::get_schema_version() const {
    return db_.current_schema_version();
}

godot::PackedStringArray StorageBridge::list_tables() const {
    godot::PackedStringArray arr;
    for (const auto& t : db_.list_tables()) {
        arr.push_back(godot::String(t.c_str()));
    }
    return arr;
}

int64_t StorageBridge::create_user_profile(const godot::String& display_name, const godot::String& avatar_key) {
    return db_.create_user_profile(display_name.utf8().get_data(), avatar_key.utf8().get_data());
}

godot::Array StorageBridge::list_user_profiles() const {
    godot::Array arr;
    for (const auto& p : db_.list_user_profiles()) {
        godot::Dictionary d;
        d["user_id"] = p.user_id;
        d["display_name"] = godot::String(p.display_name.c_str());
        d["avatar_key"] = godot::String(p.avatar_key.c_str());
        d["created_at_ms"] = p.created_at_ms;
        arr.push_back(d);
    }
    return arr;
}

bool StorageBridge::set_user_setting(int64_t user_id, const godot::String& key, const godot::String& value) {
    return db_.set_user_setting(user_id, key.utf8().get_data(), value.utf8().get_data());
}

godot::String StorageBridge::get_user_setting(int64_t user_id, const godot::String& key, const godot::String& fallback) const {
    const auto val = db_.get_user_setting(user_id, key.utf8().get_data(), fallback.utf8().get_data());
    return godot::String(val.c_str());
}

int64_t StorageBridge::create_project(int64_t user_id, const godot::String& title, const godot::String& description,
                                      int32_t width, int32_t height) {
    living_canvas::storage::ProjectRepository repo(db_);
    return repo.create_project(user_id, title.utf8().get_data(), description.utf8().get_data(), width, height);
}

godot::Array StorageBridge::list_projects(int64_t user_id) const {
    godot::Array arr;
    living_canvas::storage::ProjectRepository repo(const_cast<living_canvas::storage::DatabaseManager&>(db_));
    for (const auto& p : repo.list_projects(user_id)) {
        godot::Dictionary d;
        d["project_id"] = p.project_id;
        d["user_id"] = p.user_id;
        d["title"] = godot::String(p.title.c_str());
        d["description"] = godot::String(p.description.c_str());
        d["created_at_ms"] = p.created_at_ms;
        d["updated_at_ms"] = p.updated_at_ms;
        arr.push_back(d);
    }
    return arr;
}

bool StorageBridge::delete_project(int64_t project_id) {
    living_canvas::storage::ProjectRepository repo(db_);
    return repo.delete_project(project_id);
}

bool StorageBridge::save_project_drawing(int64_t project_id, DrawingBridge* drawing_bridge) {
    if (drawing_bridge == nullptr) {
        return false;
    }
    living_canvas::storage::ProjectRepository repo(db_);
    return repo.save_drawing_strokes(project_id, drawing_bridge->native_engine());
}

bool StorageBridge::load_project_drawing(int64_t project_id, DrawingBridge* drawing_bridge) {
    if (drawing_bridge == nullptr) {
        return false;
    }
    living_canvas::storage::ProjectRepository repo(db_);
    return repo.load_drawing_strokes(project_id, drawing_bridge->native_engine());
}

bool StorageBridge::create_backup(const godot::String& backup_file_path) {
    return db_.create_backup(backup_file_path.utf8().get_data());
}

void StorageBridge::_bind_methods() {
    using namespace godot;
    ClassDB::bind_method(D_METHOD("open_database", "absolute_db_path"), &StorageBridge::open_database);
    ClassDB::bind_method(D_METHOD("close_database"), &StorageBridge::close_database);
    ClassDB::bind_method(D_METHOD("is_open"), &StorageBridge::is_open);
    ClassDB::bind_method(D_METHOD("get_schema_version"), &StorageBridge::get_schema_version);
    ClassDB::bind_method(D_METHOD("list_tables"), &StorageBridge::list_tables);

    ClassDB::bind_method(D_METHOD("create_user_profile", "display_name", "avatar_key"), &StorageBridge::create_user_profile);
    ClassDB::bind_method(D_METHOD("list_user_profiles"), &StorageBridge::list_user_profiles);
    ClassDB::bind_method(D_METHOD("set_user_setting", "user_id", "key", "value"), &StorageBridge::set_user_setting);
    ClassDB::bind_method(D_METHOD("get_user_setting", "user_id", "key", "fallback"), &StorageBridge::get_user_setting);

    ClassDB::bind_method(D_METHOD("create_project", "user_id", "title", "description", "width", "height"), &StorageBridge::create_project);
    ClassDB::bind_method(D_METHOD("list_projects", "user_id"), &StorageBridge::list_projects);
    ClassDB::bind_method(D_METHOD("delete_project", "project_id"), &StorageBridge::delete_project);
    ClassDB::bind_method(D_METHOD("save_project_drawing", "project_id", "drawing_bridge"), &StorageBridge::save_project_drawing);
    ClassDB::bind_method(D_METHOD("load_project_drawing", "project_id", "drawing_bridge"), &StorageBridge::load_project_drawing);
    ClassDB::bind_method(D_METHOD("create_backup", "backup_file_path"), &StorageBridge::create_backup);
}

} // namespace living_canvas::gdext
