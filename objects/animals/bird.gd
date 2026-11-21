class_name BirdNodeGD
extends AnimalNodeGD

func _init() -> void:
	species = "bird"

func step_flight(move_x: float, flap: bool, dt: float) -> void:
	if AppState.physics_bridge != null:
		AppState.physics_bridge.step_bird(object_id, move_x, flap, dt)
		var st := AppState.physics_bridge.get_body_state(object_id)
		position = Vector2(float(st.get("x", position.x)), float(st.get("y", position.y)))

