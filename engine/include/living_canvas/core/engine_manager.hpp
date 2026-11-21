#ifndef LIVING_CANVAS_CORE_ENGINE_MANAGER_HPP
#define LIVING_CANVAS_CORE_ENGINE_MANAGER_HPP

#include "living_canvas/core/engine_config.hpp"
#include "living_canvas/core/event_system.hpp"

#include <memory>
#include <string>

namespace living_canvas::core {

class EngineManager {
public:
    static EngineManager& instance();

    bool initialize(const std::string& config_json = "");
    void shutdown();

    bool is_initialized() const noexcept { return initialized_; }
    const EngineConfig& config() const noexcept { return config_; }
    EventSystem& events() noexcept { return events_; }
    std::string status_summary_json() const;

private:
    EngineManager() = default;
    ~EngineManager() = default;
    EngineManager(const EngineManager&) = delete;
    EngineManager& operator=(const EngineManager&) = delete;

    bool initialized_ = false;
    EngineConfig config_;
    EventSystem events_;
};

} // namespace living_canvas::core

#endif // LIVING_CANVAS_CORE_ENGINE_MANAGER_HPP

