extends SceneTree

func _init() -> void:
	var db := DrawingBridge.new()
	var rb := RecognitionBridge.new()
	# Draw a stickman: top closed circle head + torso + 2 arms + 2 legs
	var s_head := db.begin_stroke(1, 500.0, 180.0, 1.0)
	db.append_stroke_point(s_head, 520.0, 200.0, 1.0)
	db.append_stroke_point(s_head, 500.0, 220.0, 1.0)
	db.append_stroke_point(s_head, 480.0, 200.0, 1.0)
	db.append_stroke_point(s_head, 495.0, 182.0, 1.0)
	db.append_stroke_point(s_head, 500.0, 180.0, 1.0)
	db.end_stroke(s_head)

	var s_body := db.begin_stroke(1, 500.0, 220.0, 1.0)
	db.append_stroke_point(s_body, 500.0, 340.0, 1.0)
	db.end_stroke(s_body)

	var s_larm := db.begin_stroke(1, 500.0, 250.0, 1.0)
	db.append_stroke_point(s_larm, 450.0, 290.0, 1.0)
	db.end_stroke(s_larm)

	var s_rarm := db.begin_stroke(1, 500.0, 250.0, 1.0)
	db.append_stroke_point(s_rarm, 550.0, 290.0, 1.0)
	db.end_stroke(s_rarm)

	var s_lleg := db.begin_stroke(1, 500.0, 340.0, 1.0)
	db.append_stroke_point(s_lleg, 460.0, 430.0, 1.0)
	db.end_stroke(s_lleg)

	var s_rleg := db.begin_stroke(1, 500.0, 340.0, 1.0)
	db.append_stroke_point(s_rleg, 540.0, 430.0, 1.0)
	db.end_stroke(s_rleg)

	var res = JSON.parse_string(rb.classify_drawing(db))
	assert(typeof(res) == TYPE_DICTIONARY, "Classifier output must be valid JSON")
	assert(res["category"] == "stickman", "Offline classifier must recognize drawn stickman")
	assert(float(res["confidence"]) > 0.75, "Confidence must exceed 0.75")
	assert(res["suggested_parts"].size() >= 4, "Classifier must suggest body parts")
	print("[PASS] test_recognition.gd")
	quit(0)

