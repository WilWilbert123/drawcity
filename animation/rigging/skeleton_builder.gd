class_name SkeletonBuilderGD
extends RefCounted

static func build_stickman_skeleton(object_id: int, bounds: Rect2) -> int:
	if AppState.animation_bridge == null:
		return 0
	return AppState.animation_bridge.create_auto_stickman_rig(
		object_id, bounds.position.x, bounds.position.y, bounds.end.x, bounds.end.y
	)

