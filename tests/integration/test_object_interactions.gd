extends SceneTree

func _init() -> void:
	var pb := PhysicsBridge.new()
	pb.set_environment(980.0, 620.0, 520.0, 0.0)
	var rel_id := pb.mount_vehicle(1, 2, 0.0, -24.0)
	assert(rel_id > 0, "Mounting stickman into drawn car must create relationship")
	pb.step_car(2, 1.0, 0.0, 0.0, 26.0, 0.5)
	pb.step_world(0.5)
	var car_st := pb.get_body_state(2)
	var char_st := pb.get_body_state(1)
	assert(absf(float(char_st["x"]) - float(car_st["x"])) < 0.01, "Mounted stickman must move with drawn car")
	var exited := pb.exit_vehicle(1, 2)
	assert(exited, "Stickman must be able to exit drawn car")
	print("[PASS] test_object_interactions.gd")
	quit(0)

