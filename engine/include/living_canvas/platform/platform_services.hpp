#ifndef LIVING_CANVAS_PLATFORM_PLATFORM_SERVICES_HPP
#define LIVING_CANVAS_PLATFORM_PLATFORM_SERVICES_HPP

#include <cstdint>
#include <string>

namespace living_canvas::platform {

class PlatformServices {
public:
    static std::string platform_name();
    static std::string architecture_name();
    static bool is_android();
    static bool is_offline_capable() noexcept { return true; }
    static int64_t current_timestamp_ms();
    static bool ensure_directory_exists(const std::string& directory_path);
    static bool atomic_write_text_file(const std::string& file_path, const std::string& content);
    static std::string read_text_file(const std::string& file_path, bool* ok = nullptr);
};

} // namespace living_canvas::platform

#endif // LIVING_CANVAS_PLATFORM_PLATFORM_SERVICES_HPP

