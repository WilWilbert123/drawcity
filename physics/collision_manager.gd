class_name CollisionManagerGD
extends RefCounted

static func check_aabb_collision(a: Rect2, b: Rect2) -> bool:
	return a.intersects(b, true)

