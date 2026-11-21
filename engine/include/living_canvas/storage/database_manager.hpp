#ifndef LIVING_CANVAS_STORAGE_DATABASE_MANAGER_HPP
#define LIVING_CANVAS_STORAGE_DATABASE_MANAGER_HPP

#include <cstdint>
#include <mutex>
#include <string>
#include <vector>

struct sqlite3;

namespace living_canvas::storage {

struct UserProfileRecord {
    int64_t user_id = 0;
    std::string display_name;
    std::string avatar_key;
    int64_t created_at_ms = 0;
};

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    bool open(const std::string& db_file_path);
    void close();
    bool is_open() const noexcept { return db_ != nullptr; }

    bool execute_sql(const std::string& sql, std::string* error_out = nullptr);
    bool initialize_schema(std::string* error_out = nullptr);
    int32_t current_schema_version() const;
    std::vector<std::string> list_tables() const;

    int64_t create_user_profile(const std::string& display_name, const std::string& avatar_key);
    std::vector<UserProfileRecord> list_user_profiles() const;

    bool set_user_setting(int64_t user_id, const std::string& key, const std::string& value);
    std::string get_user_setting(int64_t user_id, const std::string& key, const std::string& fallback = "") const;

    bool create_backup(const std::string& backup_file_path, std::string* error_out = nullptr);
    sqlite3* raw_handle() noexcept { return db_; }

private:
    mutable std::mutex mutex_;
    sqlite3* db_ = nullptr;
    std::string db_path_;
};

} // namespace living_canvas::storage

#endif // LIVING_CANVAS_STORAGE_DATABASE_MANAGER_HPP

