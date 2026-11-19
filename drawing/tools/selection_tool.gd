class_name SelectionToolGD
extends RefCounted

func select_rect(min_pt: Vector2, max_pt: Vector2) -> PackedInt64Array:
	if AppState.drawing_bridge != null:
		return AppState.drawing_bridge.select_strokes_in_rect(min_pt.x, min_pt.y, max_pt.x, max_pt.y)
	return PackedInt64Array()

