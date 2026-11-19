#include "living_canvas/animation/animation_clip.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>

namespace living_canvas::animation {

Keyframe AnimationTrack::evaluate(float time_sec) const {
    if (keyframes.empty()) {
        return Keyframe{};
    }
    if (keyframes.size() == 1 || time_sec <= keyframes.front().time_sec) {
        return keyframes.front();
    }
    if (time_sec >= keyframes.back().time_sec) {
        return keyframes.back();
    }
    for (std::size_t i = 0; i + 1 < keyframes.size(); ++i) {
        if (time_sec >= keyframes[i].time_sec && time_sec <= keyframes[i + 1].time_sec) {
            return Keyframe::interpolate(keyframes[i], keyframes[i + 1], time_sec);
        }
    }
    return keyframes.back();
}

AnimationClip::AnimationClip(int64_t clip_id, const std::string& name, float duration_sec, bool loop)
    : clip_id_(clip_id), name_(name), duration_sec_(std::max(0.1f, duration_sec)), loop_(loop) {}

void AnimationClip::add_keyframe(const std::string& bone_name, const Keyframe& kf) {
    AnimationTrack* target = nullptr;
    for (auto& track : tracks_) {
        if (track.target_bone_name == bone_name) {
            target = &track;
            break;
        }
    }
    if (!target) {
        tracks_.push_back(AnimationTrack{next_track_id_++, bone_name, {}});
        target = &tracks_.back();
    }
    for (auto& existing : target->keyframes) {
        if (std::fabs(existing.time_sec - kf.time_sec) < 0.005f) {
            existing = kf;
            return;
        }
    }
    target->keyframes.push_back(kf);
    std::sort(target->keyframes.begin(), target->keyframes.end(),
              [](const Keyframe& a, const Keyframe& b) { return a.time_sec < b.time_sec; });
    duration_sec_ = std::max(duration_sec_, kf.time_sec);
}

bool AnimationClip::remove_keyframe(const std::string& bone_name, float time_sec, float tolerance) {
    for (auto& track : tracks_) {
        if (track.target_bone_name == bone_name) {
            auto old_size = track.keyframes.size();
            track.keyframes.erase(
                std::remove_if(track.keyframes.begin(), track.keyframes.end(),
                               [time_sec, tolerance](const Keyframe& k) { return std::fabs(k.time_sec - time_sec) <= tolerance; }),
                track.keyframes.end());
            return track.keyframes.size() != old_size;
        }
    }
    return false;
}

Keyframe AnimationClip::evaluate_bone(const std::string& bone_name, float time_sec) const {
    float t = time_sec;
    if (loop_ && duration_sec_ > 0.001f) {
        t = std::fmod(time_sec, duration_sec_);
        if (t < 0.0f) {
            t += duration_sec_;
        }
    } else {
        t = std::max(0.0f, std::min(duration_sec_, time_sec));
    }

    for (const auto& track : tracks_) {
        if (track.target_bone_name == bone_name) {
            return track.evaluate(t);
        }
    }
    return Keyframe{t, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f};
}

std::string AnimationClip::to_json() const {
    std::ostringstream out;
    out << "{"
        << "\"id\":" << clip_id_ << ","
        << "\"name\":\"" << name_ << "\","
        << "\"duration_sec\":" << duration_sec_ << ","
        << "\"loop\":" << (loop_ ? "true" : "false") << ","
        << "\"tracks\":[";
    for (std::size_t i = 0; i < tracks_.size(); ++i) {
        if (i > 0) {
            out << ",";
        }
        out << "{"
            << "\"bone\":\"" << tracks_[i].target_bone_name << "\","
            << "\"keyframes\":[";
        for (std::size_t j = 0; j < tracks_[i].keyframes.size(); ++j) {
            if (j > 0) {
                out << ",";
            }
            out << tracks_[i].keyframes[j].to_json();
        }
        out << "]}";
    }
    out << "]}";
    return out.str();
}

} // namespace living_canvas::animation

