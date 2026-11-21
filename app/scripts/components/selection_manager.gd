class_name SelectionManagerComponent
extends RefCounted

var selected_stroke_ids: PackedInt64Array = PackedInt64Array()

func select_in_box(rect: Rect2) -> PackedInt64Array:
	if AppState.drawing_bridge != null:
		selected_stroke_ids = AppState.drawing_bridge.select_strokes_in_rect(
			rect.position.x, rect.position.y, rect.end.x, rect.end.y
		)
	return selected_stroke_ids

