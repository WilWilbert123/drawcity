class_name PhysicsWorldGD
extends Node

func configure_world(gravity_px: float = 980.0, ground_y: float = 620.0, water_y: float = 520.0, wind_vx: float = 20.0) -> void:
	if AppState.physics_bridge != null:
		AppState.physics_bridge.set_environment(gravity_px, ground_y, water_y, wind_vx)

