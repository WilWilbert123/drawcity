#include "living_canvas/history/history_manager.hpp"

namespace living_canvas::history {

HistoryManager::HistoryManager(std::size_t max_depth) : max_depth_(max_depth > 2 ? max_depth : 10) {}

void HistoryManager::push_snapshot(const std::string& action_label, const std::string& state_json) {
    undo_stack_.push_back(HistorySnapshot{next_snapshot_id_++, action_label, state_json});
    if (undo_stack_.size() > max_depth_) {
        undo_stack_.erase(undo_stack_.begin());
    }
    redo_stack_.clear();
}

bool HistoryManager::can_undo() const noexcept {
    return undo_stack_.size() > 1;
}

bool HistoryManager::can_redo() const noexcept {
    return !redo_stack_.empty();
}

HistorySnapshot HistoryManager::undo() {
    if (!can_undo()) {
        return undo_stack_.empty() ? HistorySnapshot{} : undo_stack_.back();
    }
    redo_stack_.push_back(undo_stack_.back());
    undo_stack_.pop_back();
    return undo_stack_.back();
}

HistorySnapshot HistoryManager::redo() {
    if (!can_redo()) {
        return undo_stack_.empty() ? HistorySnapshot{} : undo_stack_.back();
    }
    undo_stack_.push_back(redo_stack_.back());
    redo_stack_.pop_back();
    return undo_stack_.back();
}

void HistoryManager::clear() {
    undo_stack_.clear();
    redo_stack_.clear();
}

} // namespace living_canvas::history

