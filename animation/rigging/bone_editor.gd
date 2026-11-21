class_name BoneEditorGD
extends RefCounted

func rotate_bone(rig_id: int, bone_id: int, angle_deg: float) -> bool:
	if AppState.animation_bridge == null:
		return false
	return AppState.animation_bridge.rotate_bone(rig_id, bone_id, angle_deg)

