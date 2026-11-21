#ifndef LIVING_CANVAS_CORE_ENGINE_CONFIG_HPP
#define LIVING_CANVAS_CORE_ENGINE_CONFIG_HPP

#include <cstdint>
#include <string>

namespace living_canvas::core {

struct EngineConfigData {
    std::string engine_version = "1.0.0";
    std::string cpp_standard = "C++17";
    int32_t worker_threads = 2;
    bool enable_event_logging = true;
    bool enable_stroke_cache = true;
    int32_t max_cached_layers = 16;
    int32_t fixed_physics_hz = 60;
    int32_t memory_limit_mb = 512;
    std::string database_path = "user://data/living_canvas.db";
};

class EngineConfig {
public:
    EngineConfig();
    ~EngineConfig() = default;

    bool load_from_json_string(const std::string& json_text);
    std::string to_json_string() const;

    const EngineConfigData& data() const noexcept { return data_; }
    EngineConfigData& mutable_data() noexcept { return data_; }

private:
    EngineConfigData data_;
};

} // namespace living_canvas::core

#endif // LIVING_CANVAS_CORE_ENGINE_CONFIG_HPP

