extends SceneTree

func _init() -> void:
	var db := DrawingBridge.new()
	var t0 := Time.get_ticks_usec()
	for s in range(100):
		var sid := db.begin_stroke(1, float(s * 5), 100.0, 1.0)
		for p in range(25):
			db.append_stroke_point(sid, float(s * 5 + p), float(100 + p * 2), 0.85)
		db.end_stroke(sid)
	var elapsed_ms := float(Time.get_ticks_usec() - t0) / 1000.0
	assert(db.get_stroke_count() == 100, "100 strokes (2,600 points) must be stored")
	assert(elapsed_ms < 250.0, "100 strokes must process in under 250 ms")
	print("[PASS] test_canvas_performance.gd (100 strokes / 2,600 pts in %.2f ms)" % elapsed_ms)
	quit(0)

