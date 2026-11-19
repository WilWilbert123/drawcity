class_name RigController
extends RefCounted

func auto_rig_stickman(object_id: int, bounds: Rect2) -> int:
	if AppState.animation_bridge == null:
		return 0
	var rig_id := AppState.animation_bridge.create_auto_stickman_rig(
		object_id, bounds.position.x, bounds.position.y, bounds.end.x, bounds.end.y
	)
	AppState.active_stickman_rig_id = rig_id
	EventBus.rig_updated.emit(rig_id)
	return rig_id

func get_rig_data(rig_id: int) -> Dictionary:
	if AppState.animation_bridge == null or rig_id <= 0:
		return {}
	var parsed = JSON.parse_string(AppState.animation_bridge.serialize_rig_json(rig_id))
	return parsed if typeof(parsed) == TYPE_DICTIONARY else {}

