class_name StickmanNodeGD
extends CharacterNodeGD

func enter_vehicle(vehicle_id: int) -> int:
	if AppState.physics_bridge != null:
		return AppState.physics_bridge.mount_vehicle(object_id, vehicle_id, 0.0, -24.0)
	return 0

func exit_vehicle(vehicle_id: int) -> bool:
	if AppState.physics_bridge != null:
		return AppState.physics_bridge.exit_vehicle(object_id, vehicle_id)
	return false

