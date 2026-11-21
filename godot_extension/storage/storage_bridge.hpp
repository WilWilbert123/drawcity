#ifndef LIVING_CANVAS_GDEXT_STORAGE_BRIDGE_HPP
#define LIVING_CANVAS_GDEXT_STORAGE_BRIDGE_HPP

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/string.hpp>

#include "drawing/drawing_bridge.hpp"
#include "living_canvas/storage/database_manager.hpp"
#include "living_canvas/storage/project_repository.hpp"

namespace living_canvas::gdext {

class StorageBridge : public godot::RefCounted {
    GDCLASS(StorageBridge, godot::RefCounted)

public:
    StorageBridge() = default;
    ~StorageBridge() override = default;

    bool open_database(const godot::String& absolute_db_path);
    void close_database();
    bool is_open() const;
    int32_t get_schema_version() const;
    godot::PackedStringArray list_tables() const;

    int64_t create_user_profile(const godot::String& display_name, const godot::String& avatar_key);
    godot::Array list_user_profiles() const;
    bool set_user_setting(int64_t user_id, const godot::String& key, const godot::String& value);
    godot::String get_user_setting(int64_t user_id, const godot::String& key, const godot::String& fallback) const;

    int64_t create_project(int64_t user_id, const godot::String& title, const godot::String& description,
                           int32_t width, int32_t height);
    godot::Array list_projects(int64_t user_id) const;
    bool delete_project(int64_t project_id);

    bool save_project_drawing(int64_t project_id, DrawingBridge* drawing_bridge);
    bool load_project_drawing(int64_t project_id, DrawingBridge* drawing_bridge);
    bool create_backup(const godot::String& backup_file_path);

protected:
    static void _bind_methods();

private:
    living_canvas::storage::DatabaseManager db_;
};

} // namespace living_canvas::gdext

#endif // LIVING_CANVAS_GDEXT_STORAGE_BRIDGE_HPP
