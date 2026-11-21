class_name AnimationRepositoryGD
extends RefCounted

var _anim_bridge: AnimationBridge

func _init(anim_bridge: AnimationBridge) -> void:
	_anim_bridge = anim_bridge

func get_clip_json(clip_name: String) -> String:
	return _anim_bridge.get_clip_json(clip_name)

func serialize_rig(rig_id: int) -> String:
	return _anim_bridge.serialize_rig_json(rig_id)

