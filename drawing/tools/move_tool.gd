class_name MoveToolGD
extends RefCounted

func move_strokes(stroke_ids: PackedInt64Array, delta: Vector2) -> void:
	if AppState.drawing_bridge != null:
		AppState.drawing_bridge.translate_strokes(stroke_ids, delta.x, delta.y)

