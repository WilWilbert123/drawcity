class_name PropertyInspectorComponent
extends RefCounted

func inspect_object(object_id: int) -> Dictionary:
	if AppState.physics_bridge != null:
		return AppState.physics_bridge.get_body_state(object_id)
	return {}

