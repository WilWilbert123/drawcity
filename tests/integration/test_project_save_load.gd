extends SceneTree

func _init() -> void:
	var sb := StorageBridge.new()
	var db_path := "/tmp/living_canvas_save_load.db"
	assert(sb.open_database(db_path), "Database must open")
	var uid := sb.create_user_profile("Project Tester", "artist_01")
	var pid := sb.create_project(uid, "Editable Stickman Project", "Preserves raw stroke data", 1920, 1080)
	assert(pid > 0, "Project must be created")

	var draw1 := DrawingBridge.new()
	draw1.set_brush_color("#EC4899")
	draw1.set_brush_size(7.5)
	var sid := draw1.begin_stroke(1, 210.0, 320.0, 0.9)
	draw1.append_stroke_point(sid, 260.0, 380.0, 1.0)
	draw1.end_stroke(sid)
	assert(sb.save_project_drawing(pid, draw1), "Saving strokes to SQLite BLOB must succeed")

	var draw2 := DrawingBridge.new()
	assert(draw2.get_stroke_count() == 0, "Fresh DrawingBridge must start empty")
	assert(sb.load_project_drawing(pid, draw2), "Loading strokes from SQLite BLOB must succeed")
	assert(draw2.get_stroke_count() == 1, "Reopened project must restore editable stroke count")
	var loaded_json = JSON.parse_string(draw2.serialize_canvas_json())
	assert(loaded_json["strokes"][0]["color"] == "#EC4899", "Reopened stroke must preserve original color")
	sb.close_database()
	print("[PASS] test_project_save_load.gd")
	quit(0)

