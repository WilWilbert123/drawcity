class_name FillToolGD
extends RefCounted

func recolor_stroke_selection(color_hex: String) -> void:
	if AppState.drawing_bridge != null:
		AppState.drawing_bridge.set_brush_color(color_hex)

