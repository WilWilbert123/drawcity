class_name CharacterNodeGD
extends Node2D

@export var object_id: int = 1
@export var rig_id: int = 0
var stroke_ids: PackedInt64Array = PackedInt64Array()

func step_movement(move_x: float, jump: bool, dt: float) -> void:
	if AppState.physics_bridge != null:
		AppState.physics_bridge.step_character(object_id, move_x, jump, dt)
		var st := AppState.physics_bridge.get_body_state(object_id)
		position = Vector2(float(st.get("x", position.x)), float(st.get("y", position.y)))

