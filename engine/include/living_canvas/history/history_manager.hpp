#ifndef LIVING_CANVAS_HISTORY_HISTORY_MANAGER_HPP
#define LIVING_CANVAS_HISTORY_HISTORY_MANAGER_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace living_canvas::history {

struct HistorySnapshot {
    int64_t snapshot_id = 0;
    std::string action_label;
    std::string state_json;
};

class HistoryManager {
public:
    explicit HistoryManager(std::size_t max_depth = 100);
    ~HistoryManager() = default;

    void push_snapshot(const std::string& action_label, const std::string& state_json);
    bool can_undo() const noexcept;
    bool can_redo() const noexcept;
    HistorySnapshot undo();
    HistorySnapshot redo();
    void clear();

    std::size_t undo_count() const noexcept { return undo_stack_.size(); }
    std::size_t redo_count() const noexcept { return redo_stack_.size(); }

private:
    std::size_t max_depth_ = 100;
    int64_t next_snapshot_id_ = 1;
    std::vector<HistorySnapshot> undo_stack_;
    std::vector<HistorySnapshot> redo_stack_;
};

} // namespace living_canvas::history

#endif // LIVING_CANVAS_HISTORY_HISTORY_MANAGER_HPP

