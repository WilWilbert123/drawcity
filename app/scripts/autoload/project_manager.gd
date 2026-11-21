extends Node

func create_new_project(title: String, description: String = "") -> int:
	if not DatabaseManager.is_ready:
		return 0
	var pid := DatabaseManager.storage_bridge.create_project(
		AppState.current_user_id, title, description, 1920, 1080
	)
	if pid > 0:
		AppState.current_project_id = pid
		AppState.current_project_title = title
		var folder := "user://projects/project_%03d" % pid
		var dir := DirAccess.open("user://")
		if dir != null:
			dir.make_dir_recursive("projects/project_%03d/drawings" % pid)
			dir.make_dir_recursive("projects/project_%03d/animations" % pid)
			dir.make_dir_recursive("projects/project_%03d/rigs" % pid)
			dir.make_dir_recursive("projects/project_%03d/assets" % pid)
		FileManager.write_json_manifest(folder + "/project.json", {
			"project_id": pid,
			"title": title,
			"preserve_original_artwork": true
		})
		EventBus.active_project_changed.emit(pid, title)
	return pid

func save_current_project() -> bool:
	if not DatabaseManager.is_ready:
		return false
	if AppState.current_project_id <= 0:
		create_new_project(AppState.current_project_title)
	return DatabaseManager.storage_bridge.save_project_drawing(
		AppState.current_project_id, AppState.drawing_bridge
	)

func load_project(project_id: int) -> bool:
	if not DatabaseManager.is_ready:
		return false
	AppState.current_project_id = project_id
	return DatabaseManager.storage_bridge.load_project_drawing(
		project_id, AppState.drawing_bridge
	)

