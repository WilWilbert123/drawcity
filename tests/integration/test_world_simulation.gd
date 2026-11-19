extends SceneTree

func _init() -> void:
	var pb := PhysicsBridge.new()
	# Set water surface at y=720 and ground at y=400 so fish at y=0..400 stays outside water
	pb.set_environment(980.0, 400.0, 720.0, 25.0)
	for i in range(15):
		pb.step_fish(10, 0.0, 0.0, 0.1)
	var fish_st := pb.get_body_state(10)
	assert(float(fish_st["oxygen_remaining_sec"]) < 12.0, "Fish outside water must lose oxygen")
	# Bird flapping gains altitude
	pb.step_bird(11, 1.0, true, 0.1)
	var bird_st := pb.get_body_state(11)
	assert(float(bird_st["vy"]) < 0.0, "Flapping bird must gain upward velocity")
	print("[PASS] test_world_simulation.gd")
	quit(0)
