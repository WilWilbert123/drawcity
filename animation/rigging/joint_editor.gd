class_name JointEditorGD
extends RefCounted

func move_joint(rig_id: int, joint_id: int, pos: Vector2) -> bool:
	if AppState.animation_bridge == null:
		return false
	return AppState.animation_bridge.move_joint(rig_id, joint_id, pos.x, pos.y)

