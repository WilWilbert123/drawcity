class_name SQLiteManager
extends RefCounted

var storage_bridge: StorageBridge
var db_path: String = "user://data/living_canvas.db"

func _init() -> void:
	storage_bridge = StorageBridge.new()

func open_default_database() -> bool:
	var dir := DirAccess.open("user://")
	if dir != null and not dir.dir_exists("data"):
		dir.make_dir_recursive("data")
	var abs_path := ProjectSettings.globalize_path(db_path)
	return storage_bridge.open_database(abs_path)

func get_schema_version() -> int:
	return storage_bridge.get_schema_version()

func list_tables() -> PackedStringArray:
	return storage_bridge.list_tables()

func create_backup(backup_rel_path: String = "user://data/backups/living_canvas_backup.db") -> bool:
	var dir := DirAccess.open("user://")
	if dir != null and not dir.dir_exists("data/backups"):
		dir.make_dir_recursive("data/backups")
	var abs_backup := ProjectSettings.globalize_path(backup_rel_path)
	return storage_bridge.create_backup(abs_backup)

