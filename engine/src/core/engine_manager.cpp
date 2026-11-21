#include "living_canvas/core/engine_manager.hpp"

#include <sstream>

namespace living_canvas::core {

EngineManager& EngineManager::instance() {
    static EngineManager manager;
    return manager;
}

bool EngineManager::initialize(const std::string& config_json) {
    if (!config_json.empty()) {
        config_.load_from_json_string(config_json);
    }
    initialized_ = true;
    events_.publish("engine.initialized", config_.to_json_string());
    return true;
}

void EngineManager::shutdown() {
    if (initialized_) {
        events_.publish("engine.shutdown", "{\"status\":\"stopped\"}");
        initialized_ = false;
    }
}

std::string EngineManager::status_summary_json() const {
    std::ostringstream out;
    out << "{"
        << "\"initialized\":" << (initialized_ ? "true" : "false") << ","
        << "\"version\":\"" << config_.data().engine_version << "\","
        << "\"cpp_standard\":\"" << config_.data().cpp_standard << "\","
        << "\"fixed_physics_hz\":" << config_.data().fixed_physics_hz << ","
        << "\"recent_events\":" << events_.recent_events().size()
        << "}";
    return out.str();
}

} // namespace living_canvas::core

