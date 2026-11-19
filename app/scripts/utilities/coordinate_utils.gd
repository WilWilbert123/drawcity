class_name CoordinateUtils
extends RefCounted

static func screen_to_canvas(screen_pos: Vector2, pan_offset: Vector2, zoom_level: float) -> Vector2:
	var safe_zoom := maxf(0.1, zoom_level)
	return (screen_pos - pan_offset) / safe_zoom

static func canvas_to_screen(canvas_pos: Vector2, pan_offset: Vector2, zoom_level: float) -> Vector2:
	return canvas_pos * zoom_level + pan_offset

