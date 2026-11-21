class_name TigerNodeGD
extends AnimalNodeGD

func _init() -> void:
	species = "tiger"

func step_locomotion(move_x: float, jump: bool, dt: float) -> void:
	if AppState.physics_bridge != null:
		AppState.physics_bridge.step_character(object_id, move_x * 1.35, jump, dt)
		var st := AppState.physics_bridge.get_body_state(object_id)
		position = Vector2(float(st.get("x", position.x)), float(st.get("y", position.y)))

