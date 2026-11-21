class_name WorldController
extends RefCounted

var is_simulating: bool = false

func toggle_stickman_in_car() -> bool:
	if AppState.physics_bridge == null:
		return false
	AppState.stickman_in_car = not AppState.stickman_in_car
	if AppState.stickman_in_car:
		AppState.physics_bridge.mount_vehicle(AppState.active_stickman_object_id, AppState.active_car_object_id, 0.0, -24.0)
		EventBus.vehicle_mounted.emit(AppState.active_stickman_object_id, AppState.active_car_object_id)
	else:
		AppState.physics_bridge.exit_vehicle(AppState.active_stickman_object_id, AppState.active_car_object_id)
	return AppState.stickman_in_car

func step_simulation(delta: float, move_x: float, jump: bool, throttle: float, brake: float, steer: float) -> void:
	if not is_simulating or AppState.physics_bridge == null:
		return
	if AppState.stickman_in_car:
		AppState.physics_bridge.step_car(AppState.active_car_object_id, throttle, brake, steer, 26.0, delta)
	else:
		AppState.physics_bridge.step_character(AppState.active_stickman_object_id, move_x, jump, delta)
	AppState.physics_bridge.step_world(delta)

