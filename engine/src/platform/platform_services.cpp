#include "living_canvas/platform/platform_services.hpp"

#include <chrono>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

namespace living_canvas::platform {

namespace {

bool make_single_dir(const std::string& path) {
    if (path.empty()) {
        return false;
    }
    struct stat st {};
    if (stat(path.c_str(), &st) == 0) {
        return S_ISDIR(st.st_mode);
    }
#if defined(_WIN32)
    return _mkdir(path.c_str()) == 0;
#else
    return mkdir(path.c_str(), 0755) == 0;
#endif
}

} // namespace

std::string PlatformServices::platform_name() {
#if defined(__ANDROID__)
    return "Android";
#elif defined(__APPLE__)
    return "macOS";
#elif defined(__linux__)
    return "Linux";
#elif defined(_WIN32)
    return "Windows";
#else
    return "Unknown";
#endif
}

std::string PlatformServices::architecture_name() {
#if defined(__aarch64__) || defined(_M_ARM64)
    return "arm64";
#elif defined(__arm__) || defined(_M_ARM)
    return "arm32";
#elif defined(__x86_64__) || defined(_M_X64)
    return "x86_64";
#elif defined(__i386__) || defined(_M_IX86)
    return "x86_32";
#else
    return "unknown";
#endif
}

bool PlatformServices::is_android() {
#if defined(__ANDROID__)
    return true;
#else
    return false;
#endif
}

int64_t PlatformServices::current_timestamp_ms() {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}

bool PlatformServices::ensure_directory_exists(const std::string& directory_path) {
    if (directory_path.empty()) {
        return false;
    }
    std::string current;
    for (std::size_t i = 0; i < directory_path.size(); ++i) {
        char c = directory_path[i];
        current.push_back(c);
        if ((c == '/' || c == '\\') && current.size() > 1) {
            make_single_dir(current);
        }
    }
    return make_single_dir(current);
}

bool PlatformServices::atomic_write_text_file(const std::string& file_path, const std::string& content) {
    if (file_path.empty()) {
        return false;
    }
    auto last_slash = file_path.find_last_of("/\\");
    if (last_slash != std::string::npos && last_slash > 0) {
        ensure_directory_exists(file_path.substr(0, last_slash));
    }
    const std::string temp_path = file_path + ".tmp";
    {
        std::ofstream out(temp_path, std::ios::binary | std::ios::trunc);
        if (!out.is_open()) {
            return false;
        }
        out.write(content.data(), static_cast<std::streamsize>(content.size()));
        out.flush();
        if (!out.good()) {
            return false;
        }
    }
    std::remove(file_path.c_str());
    return std::rename(temp_path.c_str(), file_path.c_str()) == 0;
}

std::string PlatformServices::read_text_file(const std::string& file_path, bool* ok) {
    std::ifstream in(file_path, std::ios::binary);
    if (!in.is_open()) {
        if (ok) {
            *ok = false;
        }
        return "";
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    if (ok) {
        *ok = true;
    }
    return ss.str();
}

} // namespace living_canvas::platform

