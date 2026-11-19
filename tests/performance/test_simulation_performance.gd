extends SceneTree

func _init() -> void:
	var pb := PhysicsBridge.new()
	pb.set_environment(980.0, 620.0, 520.0, 20.0)
	var t0 := Time.get_ticks_usec()
	for frame in range(120):
		for obj_id in range(1, 21):
			pb.step_character(obj_id, 0.5, false, 1.0 / 60.0)
		pb.step_world(1.0 / 60.0)
	var elapsed_ms := float(Time.get_ticks_usec() - t0) / 1000.0
	var avg_frame_ms := elapsed_ms / 120.0
	assert(avg_frame_ms < 4.0, "Physics step must remain well below 16.6ms (60 FPS target)")
	print("[PASS] test_simulation_performance.gd (120 frames x 20 objects, avg %.3f ms/frame)" % avg_frame_ms)
	quit(0)

