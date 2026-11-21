#include "living_canvas/core/engine_config.hpp"

#include <sstream>

namespace living_canvas::core {

namespace {

std::string extract_string_value(const std::string& src, const std::string& key, const std::string& fallback) {
    const std::string pattern = "\"" + key + "\"";
    auto pos = src.find(pattern);
    if (pos == std::string::npos) {
        return fallback;
    }
    auto colon = src.find(':', pos + pattern.size());
    if (colon == std::string::npos) {
        return fallback;
    }
    auto first_quote = src.find('"', colon + 1);
    if (first_quote == std::string::npos) {
        return fallback;
    }
    auto second_quote = src.find('"', first_quote + 1);
    if (second_quote == std::string::npos) {
        return fallback;
    }
    return src.substr(first_quote + 1, second_quote - first_quote - 1);
}

int32_t extract_int_value(const std::string& src, const std::string& key, int32_t fallback) {
    const std::string pattern = "\"" + key + "\"";
    auto pos = src.find(pattern);
    if (pos == std::string::npos) {
        return fallback;
    }
    auto colon = src.find(':', pos + pattern.size());
    if (colon == std::string::npos) {
        return fallback;
    }
    auto start = src.find_first_of("-0123456789", colon + 1);
    if (start == std::string::npos) {
        return fallback;
    }
    auto end = src.find_first_not_of("0123456789", start + 1);
    std::string num_str = src.substr(start, end - start);
    char* end_ptr = nullptr;
    long val = std::strtol(num_str.c_str(), &end_ptr, 10);
    if (end_ptr == num_str.c_str()) {
        return fallback;
    }
    return static_cast<int32_t>(val);
}

bool extract_bool_value(const std::string& src, const std::string& key, bool fallback) {
    const std::string pattern = "\"" + key + "\"";
    auto pos = src.find(pattern);
    if (pos == std::string::npos) {
        return fallback;
    }
    auto colon = src.find(':', pos + pattern.size());
    if (colon == std::string::npos) {
        return fallback;
    }
    auto true_pos = src.find("true", colon + 1);
    auto false_pos = src.find("false", colon + 1);
    auto comma_pos = src.find_first_of(",}", colon + 1);
    if (true_pos != std::string::npos && (comma_pos == std::string::npos || true_pos < comma_pos)) {
        return true;
    }
    if (false_pos != std::string::npos && (comma_pos == std::string::npos || false_pos < comma_pos)) {
        return false;
    }
    return fallback;
}

} // namespace

EngineConfig::EngineConfig() = default;

bool EngineConfig::load_from_json_string(const std::string& json_text) {
    if (json_text.empty()) {
        return false;
    }
    data_.engine_version = extract_string_value(json_text, "engine_version", data_.engine_version);
    data_.cpp_standard = extract_string_value(json_text, "cpp_standard", data_.cpp_standard);
    data_.worker_threads = extract_int_value(json_text, "worker_threads", data_.worker_threads);
    data_.enable_event_logging = extract_bool_value(json_text, "enable_event_logging", data_.enable_event_logging);
    data_.enable_stroke_cache = extract_bool_value(json_text, "enable_stroke_cache", data_.enable_stroke_cache);
    data_.max_cached_layers = extract_int_value(json_text, "max_cached_layers", data_.max_cached_layers);
    data_.fixed_physics_hz = extract_int_value(json_text, "fixed_physics_hz", data_.fixed_physics_hz);
    data_.memory_limit_mb = extract_int_value(json_text, "memory_limit_mb", data_.memory_limit_mb);
    data_.database_path = extract_string_value(json_text, "database_path", data_.database_path);
    return true;
}

std::string EngineConfig::to_json_string() const {
    std::ostringstream out;
    out << "{\n"
        << "  \"engine_version\": \"" << data_.engine_version << "\",\n"
        << "  \"cpp_standard\": \"" << data_.cpp_standard << "\",\n"
        << "  \"worker_threads\": " << data_.worker_threads << ",\n"
        << "  \"enable_event_logging\": " << (data_.enable_event_logging ? "true" : "false") << ",\n"
        << "  \"enable_stroke_cache\": " << (data_.enable_stroke_cache ? "true" : "false") << ",\n"
        << "  \"max_cached_layers\": " << data_.max_cached_layers << ",\n"
        << "  \"fixed_physics_hz\": " << data_.fixed_physics_hz << ",\n"
        << "  \"memory_limit_mb\": " << data_.memory_limit_mb << ",\n"
        << "  \"database_path\": \"" << data_.database_path << "\"\n"
        << "}";
    return out.str();
}

} // namespace living_canvas::core

