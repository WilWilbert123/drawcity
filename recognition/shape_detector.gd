class_name ShapeDetectorGD
extends RefCounted

static func is_closed_loop(points: PackedVector2Array, threshold: float = 28.0) -> bool:
	if points.size() < 6:
		return false
	return points[0].distance_to(points[points.size() - 1]) <= threshold

