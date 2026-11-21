class_name CarNodeGD
extends VehicleNodeGD

var wheel_rotation_rad: float = 0.0

func _init() -> void:
	vehicle_type = "car"

func step_drive(throttle: float, brake: float, steer: float, dt: float) -> void:
	if AppState.physics_bridge != null:
		AppState.physics_bridge.step_car(object_id, throttle, brake, steer, 26.0, dt)
		var st := AppState.physics_bridge.get_body_state(object_id)
		position = Vector2(float(st.get("x", position.x)), float(st.get("y", position.y)))
		wheel_rotation_rad = float(st.get("wheel_rotation_rad", 0.0))

