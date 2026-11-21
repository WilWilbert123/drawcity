class_name ProjectRepositoryGD
extends RefCounted

var _bridge: StorageBridge

func _init(bridge: StorageBridge) -> void:
	_bridge = bridge

func create_project(user_id: int, title: String, description: String = "", width: int = 1920, height: int = 1080) -> int:
	return _bridge.create_project(user_id, title, description, width, height)

func list_projects(user_id: int = 0) -> Array:
	return _bridge.list_projects(user_id)

func delete_project(project_id: int) -> bool:
	return _bridge.delete_project(project_id)

