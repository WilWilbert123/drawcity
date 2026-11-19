class_name EditorController
extends RefCounted

var toolbar := DrawingToolbarComponent.new()
var layers := LayerManagerComponent.new()
var selection := SelectionManagerComponent.new()

func set_brush_color(hex_color: String) -> void:
	if AppState.drawing_bridge != null:
		AppState.drawing_bridge.set_brush_color(hex_color)

func set_brush_size(size_px: float) -> void:
	if AppState.drawing_bridge != null:
		AppState.drawing_bridge.set_brush_size(size_px)

