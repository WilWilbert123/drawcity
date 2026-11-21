class_name HomeController
extends RefCounted

func list_user_projects() -> Array:
	if not DatabaseManager.is_ready:
		return []
	return DatabaseManager.storage_bridge.list_projects(AppState.current_user_id)

func create_project(title: String, description: String = "") -> int:
	return ProjectManager.create_new_project(title, description)

