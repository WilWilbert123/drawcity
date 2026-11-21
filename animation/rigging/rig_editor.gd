class_name RigEditorView
extends Control

var rig_controller := RigController.new()
var joint_editor := JointEditorGD.new()
var bone_editor := BoneEditorGD.new()

func auto_build_stickman_rig(bounds: Rect2 = Rect2(420, 200, 160, 300)) -> int:
	return rig_controller.auto_rig_stickman(AppState.active_stickman_object_id, bounds)

