class_name ObjectClassifierGD
extends RefCounted

func classify_active_drawing() -> Dictionary:
	if AppState.recognition_bridge == null or AppState.drawing_bridge == null:
		return {"category": "other", "confidence": 0.0}
	var res_json := AppState.recognition_bridge.classify_drawing(AppState.drawing_bridge)
	var parsed = JSON.parse_string(res_json)
	return parsed if typeof(parsed) == TYPE_DICTIONARY else {"category": "other", "confidence": 0.0}

