class_name ExportController
extends RefCounted

func export_project_bundle(output_rel_path: String = "user://exports/project_files/living_canvas_export.json") -> bool:
	if AppState.drawing_bridge == null:
		return false
	var payload := {
		"project_id": AppState.current_project_id,
		"title": AppState.current_project_title,
		"preserve_original_artwork": true,
		"canvas": JSON.parse_string(AppState.drawing_bridge.serialize_canvas_json())
	}
	return FileManager.write_json_manifest(output_rel_path, payload)

