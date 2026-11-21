#ifndef LIVING_CANVAS_RECOGNITION_OBJECT_CLASSIFIER_HPP
#define LIVING_CANVAS_RECOGNITION_OBJECT_CLASSIFIER_HPP

#include "living_canvas/drawing/stroke.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace living_canvas::recognition {

struct PartSuggestion {
    std::string part_name;
    int64_t stroke_id = 0;
    float confidence = 0.0f;
};

struct ClassificationResult {
    std::string category = "other";
    float confidence = 0.0f;
    bool requires_manual_confirmation = false;
    std::vector<PartSuggestion> suggested_parts;
    std::string to_json() const;
};

class ObjectClassifier {
public:
    static ClassificationResult classify_strokes(const std::vector<living_canvas::drawing::Stroke>& strokes);
    static std::vector<std::string> supported_categories();
};

} // namespace living_canvas::recognition

#endif // LIVING_CANVAS_RECOGNITION_OBJECT_CLASSIFIER_HPP

