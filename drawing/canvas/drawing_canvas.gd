class_name DrawingCanvasView
extends Control

var renderer: CanvasRenderer
var input_handler: CanvasInputHandler

func _ready() -> void:
	clip_contents = true
	renderer = CanvasRenderer.new()
	add_child(renderer)
	input_handler = CanvasInputHandler.new()
	add_child(input_handler)

func _gui_input(event: InputEvent) -> void:
	if input_handler != null:
		input_handler.handle_gui_input(event, renderer)

func refresh_canvas() -> void:
	if renderer != null:
		renderer.queue_redraw()

