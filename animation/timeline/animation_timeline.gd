class_name AnimationTimelineView
extends Control

var track_manager := TrackManagerGD.new()
var keyframe_editor := KeyframeEditorGD.new()
var anim_controller := AnimationController.new()

func play_clip(clip_name: String) -> void:
	anim_controller.play(clip_name)

func stop_clip() -> void:
	anim_controller.stop()

