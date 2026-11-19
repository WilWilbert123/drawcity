extends SceneTree

func _init() -> void:
	var ab := AnimationBridge.new()
	var rig_id := ab.create_auto_stickman_rig(1, 400.0, 200.0, 560.0, 500.0)
	ab.play_clip("walk")
	assert(ab.get_active_clip_name() == "walk", "Active clip must be walk")
	ab.step_animation(0.2, rig_id)
	assert(ab.get_current_time() > 0.19, "Timeline must advance during playback")
	var walk_json = JSON.parse_string(ab.get_clip_json("walk"))
	assert(typeof(walk_json) == TYPE_DICTIONARY and walk_json["tracks"].size() >= 4, "Walk preset must have limb tracks")
	ab.pause()
	var paused_t := ab.get_current_time()
	ab.step_animation(0.2, rig_id)
	assert(absf(float(ab.get_current_time()) - float(paused_t)) < 0.001, "Paused animation must not advance")
	ab.stop()
	assert(absf(float(ab.get_current_time())) < 0.001, "Stopped animation must reset time to 0")
	print("[PASS] test_animation.gd")
	quit(0)

