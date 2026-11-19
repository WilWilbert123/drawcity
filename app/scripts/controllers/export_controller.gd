class_name ExportController
extends RefCounted

func export_project_bundle(output_rel_path: String = "user://exports/project_files/living_canvas_export.json") -> bool:
	if AppState.drawing_bridge == null:
		return false
	var payload := {
		"project_id": AppState.current_project_id,
		"title": AppState.current_project_title,
		"preserve_original_artwork": true,
		"canvas": JSON.parse_string(AppState.drawing_bridge.serialize_canvas_json()),
		"rig": JSON.parse_string(AppState.animation_bridge.serialize_rig_json(AppState.active_stickman_rig_id)) if (AppState.animation_bridge != null and AppState.active_stickman_rig_id > 0) else {}
	}
	return FileManager.write_json_manifest(output_rel_path, payload)

func export_canvas_png(viewport: Viewport, output_rel_path: String = "user://exports/images/living_canvas_drawing.png") -> bool:
	if viewport == null or viewport.get_texture() == null:
		return false
	var img := viewport.get_texture().get_image()
	if img == null or img.is_empty():
		return false
	FileManager.ensure_user_storage_tree()
	var err := img.save_png(output_rel_path)
	return err == OK


