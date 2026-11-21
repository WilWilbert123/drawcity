class_name TransformToolGD
extends RefCounted

func scale_selection(stroke_ids: PackedInt64Array, scale_factor: Vector2, pivot: Vector2) -> void:
	if AppState.drawing_bridge != null:
		AppState.drawing_bridge.scale_strokes(stroke_ids, scale_factor.x, scale_factor.y, pivot.x, pivot.y)

func rotate_selection(stroke_ids: PackedInt64Array, angle_rad: float, pivot: Vector2) -> void:
	if AppState.drawing_bridge != null:
		AppState.drawing_bridge.rotate_strokes(stroke_ids, angle_rad, pivot.x, pivot.y)

