class_name ObjectManagerComponent
extends RefCounted

var objects: Array = []

func classify_current_drawing() -> Dictionary:
	if AppState.recognition_bridge == null or AppState.drawing_bridge == null:
		return {"category": "other", "confidence": 0.0}
	var json_str := AppState.recognition_bridge.classify_drawing(AppState.drawing_bridge)
	var parsed = JSON.parse_string(json_str)
	if typeof(parsed) == TYPE_DICTIONARY:
		return parsed
	return {"category": "other", "confidence": 0.0}

