class_name CharacterAnimatorGD
extends RefCounted

func apply_locomotion_clip(velocity_x: float, on_ground: bool) -> String:
	if not on_ground:
		return "jump"
	var speed := absf(velocity_x)
	if speed > 180.0:
		return "run"
	elif speed > 15.0:
		return "walk"
	return "idle"

