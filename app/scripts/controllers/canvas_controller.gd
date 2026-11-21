class_name CanvasController
extends RefCounted

var active_stroke_id: int = 0
var active_layer_id: int = 1
var is_eraser_active: bool = false

func begin_input(pos: Vector2, pressure: float = 1.0) -> void:
	if AppState.drawing_bridge == null:
		return
	if is_eraser_active:
		AppState.drawing_bridge.erase_near(active_layer_id, pos.x, pos.y, AppState.drawing_bridge.get_brush_size())
	else:
		active_stroke_id = AppState.drawing_bridge.begin_stroke(active_layer_id, pos.x, pos.y, pressure)

func move_input(pos: Vector2, pressure: float = 1.0) -> void:
	if AppState.drawing_bridge == null:
		return
	if is_eraser_active:
		AppState.drawing_bridge.erase_near(active_layer_id, pos.x, pos.y, AppState.drawing_bridge.get_brush_size())
	elif active_stroke_id > 0:
		AppState.drawing_bridge.append_stroke_point(active_stroke_id, pos.x, pos.y, pressure)

func end_input() -> void:
	if AppState.drawing_bridge != null and active_stroke_id > 0:
		AppState.drawing_bridge.end_stroke(active_stroke_id)
		EventBus.stroke_committed.emit(active_stroke_id)
		active_stroke_id = 0

