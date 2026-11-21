#include "living_canvas/core/event_system.hpp"

#include <algorithm>
#include <chrono>

namespace living_canvas::core {

int32_t EventSystem::subscribe(const std::string& topic, EventCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    const int32_t id = next_id_++;
    subscriptions_.push_back(Subscription{id, topic, std::move(callback)});
    return id;
}

void EventSystem::unsubscribe(int32_t subscription_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    subscriptions_.erase(
        std::remove_if(subscriptions_.begin(), subscriptions_.end(),
                       [subscription_id](const Subscription& s) { return s.id == subscription_id; }),
        subscriptions_.end());
}

void EventSystem::publish(const std::string& topic, const std::string& payload_json, int64_t timestamp_ms) {
    if (timestamp_ms <= 0) {
        auto now = std::chrono::steady_clock::now().time_since_epoch();
        timestamp_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    }

    EngineEvent event{topic, payload_json, timestamp_ms};
    std::vector<EventCallback> matching_callbacks;

    {
        std::lock_guard<std::mutex> lock(mutex_);
        event_log_.push_back(event);
        if (event_log_.size() > 256) {
            event_log_.erase(event_log_.begin());
        }
        for (const auto& sub : subscriptions_) {
            if (sub.topic == topic || sub.topic == "*") {
                matching_callbacks.push_back(sub.callback);
            }
        }
    }

    for (const auto& cb : matching_callbacks) {
        if (cb) {
            cb(event);
        }
    }
}

std::vector<EngineEvent> EventSystem::recent_events(std::size_t max_count) const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (event_log_.size() <= max_count) {
        return event_log_;
    }
    return std::vector<EngineEvent>(event_log_.end() - static_cast<std::ptrdiff_t>(max_count), event_log_.end());
}

void EventSystem::clear_history() {
    std::lock_guard<std::mutex> lock(mutex_);
    event_log_.clear();
}

} // namespace living_canvas::core

