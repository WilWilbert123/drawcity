class_name GunNodeGD
extends WeaponNodeGD

signal fired(muzzle_pos: Vector2, direction: Vector2)

func attach_to_character(character_id: int) -> int:
	holder_character_id = character_id
	if AppState.physics_bridge != null:
		return AppState.physics_bridge.attach_held_object(character_id, object_id, 26.0, -8.0)
	return 0

func trigger_fire() -> void:
	fired.emit(global_position, Vector2.RIGHT.rotated(global_rotation))

