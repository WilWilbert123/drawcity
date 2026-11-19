class_name TimelineManagerComponent
extends RefCounted

var current_fps: int = 24
var is_looping: bool = true

func play_preset(preset_name: String) -> void:
	if AppState.animation_bridge != null:
		AppState.animation_bridge.play_clip(preset_name)
		EventBus.animation_state_changed.emit(preset_name, true)

