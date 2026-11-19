extends SceneTree

func _init() -> void:
	var pb := PhysicsBridge.new()
	pb.set_environment(980.0, 600.0, 500.0, 15.0)
	pb.step_character(1, 1.0, false, 0.5)
	var st := pb.get_body_state(1)
	assert(float(st["x"]) > 10.0, "Character must move horizontally")
	assert(float(st["y"]) > 0.0, "Character must respond to gravity")
	for i in range(20):
		pb.step_character(1, 0.0, false, 0.1)
	st = pb.get_body_state(1)
	assert(bool(st["on_ground"]) and absf(float(st["y"]) - 600.0) < 0.5, "Character must collide with ground")
	pb.step_car(2, 1.0, 0.0, 0.2, 26.0, 0.5)
	var car_st := pb.get_body_state(2)
	assert(float(car_st["wheel_rotation_rad"]) > 0.1, "Car wheels must rotate during acceleration")
	print("[PASS] test_physics.gd")
	quit(0)

