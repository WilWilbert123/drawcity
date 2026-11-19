class_name ResponsiveLayout
extends RefCounted

static func compute_canvas_ratio(viewport_size: Vector2) -> float:
	if viewport_size.y <= 1.0:
		return 1.777
	return viewport_size.x / viewport_size.y

