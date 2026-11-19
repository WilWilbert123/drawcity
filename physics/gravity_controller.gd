class_name GravityControllerGD
extends RefCounted

var gravity_px_s2: float = 980.0

func apply_gravity(new_gravity: float, ground_y: float = 620.0, water_y: float = 520.0, wind_vx: float = 20.0) -> void:
	gravity_px_s2 = new_gravity
	if AppState.physics_bridge != null:
		AppState.physics_bridge.set_environment(gravity_px_s2, ground_y, water_y, wind_vx)

