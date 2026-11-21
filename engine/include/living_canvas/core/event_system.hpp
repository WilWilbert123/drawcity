#ifndef LIVING_CANVAS_CORE_EVENT_SYSTEM_HPP
#define LIVING_CANVAS_CORE_EVENT_SYSTEM_HPP

#include <cstdint>
#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace living_canvas::core {

struct EngineEvent {
    std::string topic;
    std::string payload_json;
    int64_t timestamp_ms = 0;
};

class EventSystem {
public:
    using EventCallback = std::function<void(const EngineEvent&)>;

    EventSystem() = default;
    ~EventSystem() = default;

    int32_t subscribe(const std::string& topic, EventCallback callback);
    void unsubscribe(int32_t subscription_id);
    void publish(const std::string& topic, const std::string& payload_json, int64_t timestamp_ms = 0);
    std::vector<EngineEvent> recent_events(std::size_t max_count = 32) const;
    void clear_history();

private:
    struct Subscription {
        int32_t id = 0;
        std::string topic;
        EventCallback callback;
    };

    mutable std::mutex mutex_;
    int32_t next_id_ = 1;
    std::vector<Subscription> subscriptions_;
    std::vector<EngineEvent> event_log_;
};

} // namespace living_canvas::core

#endif // LIVING_CANVAS_CORE_EVENT_SYSTEM_HPP

