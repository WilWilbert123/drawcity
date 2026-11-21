class_name DrawingRepositoryGD
extends RefCounted

var _bridge: StorageBridge

func _init(bridge: StorageBridge) -> void:
	_bridge = bridge

func save_drawing(project_id: int, drawing_bridge: DrawingBridge) -> bool:
	return _bridge.save_project_drawing(project_id, drawing_bridge)

func load_drawing(project_id: int, drawing_bridge: DrawingBridge) -> bool:
	return _bridge.load_project_drawing(project_id, drawing_bridge)

