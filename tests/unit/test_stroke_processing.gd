extends SceneTree

func _init() -> void:
	var db := DrawingBridge.new()
	var sid := db.begin_stroke(1, 50.0, 50.0, 1.0)
	db.append_stroke_point(sid, 100.0, 50.0, 1.0)
	db.append_stroke_point(sid, 150.0, 50.0, 1.0)
	db.end_stroke(sid)
	var pts := db.get_stroke_points(sid)
	assert(pts.size() == 3, "All 3 stroke points must be preserved")
	var selected := db.select_strokes_in_rect(40.0, 40.0, 160.0, 60.0)
	assert(selected.size() == 1 and selected[0] == sid, "Rect selection must find stroke")
	db.translate_strokes(selected, 25.0, 15.0)
	var moved_pts := db.get_stroke_points(sid)
	assert(absf(moved_pts[0].x - 75.0) < 0.01 and absf(moved_pts[0].y - 65.0) < 0.01, "Translation must shift original points")
	assert(db.can_undo(), "Undo history must be recorded")
	db.undo()
	assert(db.can_redo(), "Redo history must be available after undo")
	var erased := db.erase_near(1, 75.0, 65.0, 12.0)
	assert(erased == 1 and db.get_stroke_count() == 0, "Eraser must remove hit stroke")
	print("[PASS] test_stroke_processing.gd")
	quit(0)

