class_name FishNodeGD
extends AnimalNodeGD

var oxygen_remaining: float = 12.0
var is_alive: bool = true

func _init() -> void:
	species = "fish"

func step_swim(swim_x: float, swim_y: float, dt: float) -> void:
	if AppState.physics_bridge != null:
		AppState.physics_bridge.step_fish(object_id, swim_x, swim_y, dt)
		var st := AppState.physics_bridge.get_body_state(object_id)
		position = Vector2(float(st.get("x", position.x)), float(st.get("y", position.y)))
		oxygen_remaining = float(st.get("oxygen_remaining_sec", 12.0))
		is_alive = bool(st.get("alive", true))

