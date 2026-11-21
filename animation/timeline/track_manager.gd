class_name TrackManagerGD
extends RefCounted

func get_clip_tracks(clip_name: String) -> Array:
	if AppState.animation_bridge == null:
		return []
	var parsed = JSON.parse_string(AppState.animation_bridge.get_clip_json(clip_name))
	if typeof(parsed) == TYPE_DICTIONARY and parsed.has("tracks"):
		return parsed["tracks"]
	return []

