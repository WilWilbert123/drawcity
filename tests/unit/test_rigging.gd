extends SceneTree

func _init() -> void:
	var ab := AnimationBridge.new()
	var rig_id := ab.create_auto_stickman_rig(1, 400.0, 200.0, 560.0, 500.0)
	assert(rig_id > 0, "Stickman rig ID must be valid")
	var parsed = JSON.parse_string(ab.serialize_rig_json(rig_id))
	assert(typeof(parsed) == TYPE_DICTIONARY, "Serialized rig must be valid JSON")
	assert(parsed["joints"].size() == 11, "Auto stickman rig must have 11 anatomical joints")
	assert(parsed["bones"].size() == 10, "Auto stickman rig must have 10 hierarchical bones")
	var moved := ab.move_joint(rig_id, 1, 485.0, 365.0)
	assert(moved, "Joint movement must succeed")
	var ik_ok := ab.solve_two_bone_ik(rig_id, 3, 4, 420.0, 310.0)
	assert(ik_ok, "Two-bone IK solver must converge")
	var db := DrawingBridge.new()
	db.load_sample_stickman()
	assert(db.get_stroke_count() == 6, "Sample stickman must have 6 preserved strokes")
	var bound_rig_id := ab.create_auto_stickman_rig_from_drawing(1, db)
	assert(bound_rig_id > 0, "Auto rig from drawn strokes must succeed")
	var bound_parsed = JSON.parse_string(ab.serialize_rig_json(bound_rig_id))
	var total_bound := 0
	for b in bound_parsed["bones"]:
		total_bound += b["bound_stroke_ids"].size()
	assert(total_bound == 6, "All 6 original stickman strokes must be bound to skeleton bones")
	print("[PASS] test_rigging.gd")
	quit(0)

