class_name AirplaneNodeGD
extends VehicleNodeGD

var is_stalled: bool = false

func _init() -> void:
	vehicle_type = "airplane"

func step_flight(throttle: float, pitch_input: float, dt: float) -> void:
	if AppState.physics_bridge != null:
		AppState.physics_bridge.step_airplane(object_id, throttle, pitch_input, dt)
		var st := AppState.physics_bridge.get_body_state(object_id)
		position = Vector2(float(st.get("x", position.x)), float(st.get("y", position.y)))
		rotation = float(st.get("rotation_rad", 0.0))
		is_stalled = bool(st.get("stalled", false))

