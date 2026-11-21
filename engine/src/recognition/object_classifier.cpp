#include "living_canvas/recognition/object_classifier.hpp"
#include "living_canvas/recognition/shape_recognizer.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>

namespace living_canvas::recognition {

std::string ClassificationResult::to_json() const {
    std::ostringstream out;
    out << "{"
        << "\"category\":\"" << category << "\","
        << "\"confidence\":" << confidence << ","
        << "\"requires_manual_confirmation\":" << (requires_manual_confirmation ? "true" : "false") << ","
        << "\"suggested_parts\":[";
    for (std::size_t i = 0; i < suggested_parts.size(); ++i) {
        if (i > 0) {
            out << ",";
        }
        out << "{"
            << "\"part_name\":\"" << suggested_parts[i].part_name << "\","
            << "\"stroke_id\":" << suggested_parts[i].stroke_id << ","
            << "\"confidence\":" << suggested_parts[i].confidence
            << "}";
    }
    out << "]}";
    return out.str();
}

ClassificationResult ObjectClassifier::classify_strokes(const std::vector<living_canvas::drawing::Stroke>& strokes) {
    ClassificationResult result;
    if (strokes.empty()) {
        result.category = "other";
        result.confidence = 0.0f;
        result.requires_manual_confirmation = true;
        return result;
    }

    living_canvas::drawing::BoundingBox overall;
    int32_t circle_count = 0;
    int32_t line_count = 0;
    int32_t wave_count = 0;
    int64_t top_circle_stroke_id = 0;
    float top_circle_y = 1e30f;
    std::vector<int64_t> bottom_circles;

    for (const auto& s : strokes) {
        const auto desc = ShapeRecognizer::analyze_stroke(s);
        if (!desc.bounds.valid) {
            continue;
        }
        if (!overall.valid) {
            overall = desc.bounds;
        } else {
            overall.min_x = std::min(overall.min_x, desc.bounds.min_x);
            overall.max_x = std::max(overall.max_x, desc.bounds.max_x);
            overall.min_y = std::min(overall.min_y, desc.bounds.min_y);
            overall.max_y = std::max(overall.max_y, desc.bounds.max_y);
        }

        if (desc.shape == PrimitiveShape::Circle) {
            ++circle_count;
            if (desc.bounds.center_y() < top_circle_y) {
                top_circle_y = desc.bounds.center_y();
                top_circle_stroke_id = s.id();
            }
        } else if (desc.shape == PrimitiveShape::Line) {
            ++line_count;
        } else if (desc.shape == PrimitiveShape::Wave) {
            ++wave_count;
        }
    }

    if (!overall.valid) {
        result.requires_manual_confirmation = true;
        return result;
    }

    const float total_w = std::max(1.0f, overall.width());
    const float total_h = std::max(1.0f, overall.height());
    const float aspect = total_w / total_h;

    for (const auto& s : strokes) {
        const auto desc = ShapeRecognizer::analyze_stroke(s);
        if (desc.shape == PrimitiveShape::Circle &&
            desc.bounds.center_y() > overall.min_y + total_h * 0.55f) {
            bottom_circles.push_back(s.id());
        }
    }

    // Heuristic 1: Stickman / Human (tall aspect ratio, top circle head + limb lines)
    if (circle_count >= 1 && line_count >= 3 && aspect <= 1.35f &&
        top_circle_y <= overall.min_y + total_h * 0.38f) {
        result.category = "stickman";
        result.confidence = 0.86f;
        result.suggested_parts.push_back(PartSuggestion{"head", top_circle_stroke_id, 0.92f});
        for (const auto& s : strokes) {
            if (s.id() == top_circle_stroke_id) {
                continue;
            }
            const auto b = s.compute_bounds();
            if (!b.valid) {
                continue;
            }
            if (std::fabs(b.center_x() - overall.center_x()) < total_w * 0.18f &&
                b.center_y() < overall.min_y + total_h * 0.65f) {
                result.suggested_parts.push_back(PartSuggestion{"body", s.id(), 0.84f});
            } else if (b.center_y() < overall.min_y + total_h * 0.62f) {
                result.suggested_parts.push_back(
                    PartSuggestion{b.center_x() < overall.center_x() ? "left_arm" : "right_arm", s.id(), 0.80f});
            } else {
                result.suggested_parts.push_back(
                    PartSuggestion{b.center_x() < overall.center_x() ? "left_leg" : "right_leg", s.id(), 0.80f});
            }
        }
    }
    // Heuristic 2: Car / Wheeled Vehicle (horizontal aspect + 2 bottom circles for wheels)
    else if (bottom_circles.size() >= 2 && aspect >= 1.25f) {
        result.category = "car";
        result.confidence = 0.84f;
        for (std::size_t i = 0; i < bottom_circles.size(); ++i) {
            result.suggested_parts.push_back(
                PartSuggestion{i == 0 ? "wheel_left" : "wheel_right", bottom_circles[i], 0.89f});
        }
        for (const auto& s : strokes) {
            if (std::find(bottom_circles.begin(), bottom_circles.end(), s.id()) == bottom_circles.end()) {
                result.suggested_parts.push_back(PartSuggestion{"chassis", s.id(), 0.82f});
            }
        }
    }
    // Heuristic 3: Water / Ground environment (very wide aspect ratio or wave strokes)
    else if (wave_count >= 1 && aspect >= 2.8f) {
        result.category = (overall.center_y() > 500.0f) ? "water" : "cloud";
        result.confidence = 0.76f;
    } else if (aspect >= 4.0f && line_count >= 1) {
        result.category = "ground";
        result.confidence = 0.78f;
    }
    // Heuristic 4: Fish / Bird / Tiger / Airplane based on aspect and stroke topology
    else if (aspect >= 1.3f && aspect <= 2.8f && strokes.size() >= 2 && strokes.size() <= 5) {
        result.category = (circle_count >= 1) ? "fish" : "bird";
        result.confidence = 0.64f;
    } else if (aspect >= 1.2f && line_count >= 4) {
        result.category = "tiger";
        result.confidence = 0.58f;
    } else {
        result.category = "other";
        result.confidence = 0.35f;
    }

    result.requires_manual_confirmation = (result.confidence < 0.70f);
    return result;
}

std::vector<std::string> ObjectClassifier::supported_categories() {
    return {
        "human", "stickman", "animal", "bird", "fish", "tiger",
        "car", "truck", "tank", "airplane", "dragon", "weapon",
        "water", "sky", "cloud", "ground", "tree", "building", "other"
    };
}

} // namespace living_canvas::recognition

