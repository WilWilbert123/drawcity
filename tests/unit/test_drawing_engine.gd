extends SceneTree

func _init() -> void:
	var db := DrawingBridge.new()
	var lid := db.create_layer("Unit Layer")
	assert(lid >= 2, "Layer ID must increment")
	db.select_brush(0)
	db.set_brush_color("#10B981")
	db.set_brush_size(6.0)
	var sid := db.begin_stroke(1, 100.0, 120.0, 0.8)
	db.append_stroke_point(sid, 140.0, 160.0, 0.9)
	db.append_stroke_point(sid, 180.0, 200.0, 1.0)
	var ok := db.end_stroke(sid)
	assert(ok, "Stroke must end cleanly")
	assert(db.get_stroke_count() == 1, "Stroke count must be 1")
	assert(db.get_stroke_color(sid) == "#10B981", "Original stroke color must be preserved")
	assert(absf(float(db.get_stroke_width(sid)) - 6.0) < 0.01, "Original brush width must be preserved")
	print("[PASS] test_drawing_engine.gd")
	quit(0)

