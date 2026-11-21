extends Node

func ensure_user_storage_tree() -> void:
	var dir := DirAccess.open("user://")
	if dir == null:
		return
	var folders := [
		"data",
		"data/backups",
		"projects",
		"cache/thumbnails",
		"cache/rendered_frames",
		"cache/temporary",
		"exports/images",
		"exports/videos",
		"exports/project_files"
	]
	for folder in folders:
		if not dir.dir_exists(folder):
			dir.make_dir_recursive(folder)

func _ready() -> void:
	ensure_user_storage_tree()

func write_json_manifest(rel_path: String, payload: Dictionary) -> bool:
	var f := FileAccess.open(rel_path, FileAccess.WRITE)
	if f == null:
		return false
	f.store_string(JSON.stringify(payload, "  "))
	f.close()
	return true

