extends SceneTree

func _init() -> void:
	var sb := StorageBridge.new()
	var db_path := "/tmp/living_canvas_test_db.db"
	var ok := sb.open_database(db_path)
	assert(ok and sb.is_open(), "SQLite database must open and initialize schema")
	assert(sb.get_schema_version() == 8, "Schema version must be 8")
	var tables := sb.list_tables()
	assert(tables.size() >= 23, "All 23 required tables + schema_migrations must exist")
	var uid := sb.create_user_profile("Offline Artist", "avatar_02")
	assert(uid > 0, "Local user profile creation must return valid user_id")
	var set_ok := sb.set_user_setting(uid, "preferred_brush", "marker")
	assert(set_ok, "User setting upsert must succeed")
	assert(sb.get_user_setting(uid, "preferred_brush", "") == "marker", "User setting retrieval must match")
	var backup_ok := sb.create_backup("/tmp/living_canvas_test_backup.db")
	assert(backup_ok, "SQLite online backup API must succeed")
	sb.close_database()
	print("[PASS] test_database.gd")
	quit(0)

