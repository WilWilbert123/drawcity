class_name CanvasInputHandler
extends Node

var controller := CanvasController.new()
var is_pressing: bool = false

func handle_gui_input(event: InputEvent, renderer: Node2D) -> void:
	if event is InputEventScreenTouch:
		if event.pressed:
			is_pressing = true
			controller.begin_input(event.position, 1.0)
		else:
			is_pressing = false
			controller.end_input()
		if renderer != null:
			renderer.queue_redraw()
	elif event is InputEventScreenDrag and is_pressing:
		controller.move_input(event.position, event.pressure if event.pressure > 0.0 else 1.0)
		if renderer != null:
			renderer.queue_redraw()
	elif event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT:
		if event.pressed:
			is_pressing = true
			controller.begin_input(event.position, 1.0)
		else:
			is_pressing = false
			controller.end_input()
		if renderer != null:
			renderer.queue_redraw()
	elif event is InputEventMouseMotion and is_pressing:
		var p: float = float(event.pressure) if event.pressure > 0.0 else 1.0
		controller.move_input(event.position, p)
		if renderer != null:
			renderer.queue_redraw()
