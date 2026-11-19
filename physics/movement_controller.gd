class_name MovementControllerGD
extends RefCounted

func step_character(object_id: int, move_x: float, jump: bool, dt: float) -> Dictionary:
	if AppState.physics_bridge != null:
		AppState.physics_bridge.step_character(object_id, move_x, jump, dt)
		return AppState.physics_bridge.get_body_state(object_id)
	return {}

