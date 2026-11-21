class_name AnimationController
extends RefCounted

func play(clip_name: String) -> void:
	if AppState.animation_bridge != null:
		AppState.animation_bridge.play_clip(clip_name)
		EventBus.animation_state_changed.emit(clip_name, true)

func pause() -> void:
	if AppState.animation_bridge != null:
		AppState.animation_bridge.pause()
		EventBus.animation_state_changed.emit(AppState.animation_bridge.get_active_clip_name(), false)

func stop() -> void:
	if AppState.animation_bridge != null:
		AppState.animation_bridge.stop()
		EventBus.animation_state_changed.emit(AppState.animation_bridge.get_active_clip_name(), false)

func step(delta: float) -> void:
	if AppState.animation_bridge != null:
		AppState.animation_bridge.step_animation(delta, AppState.active_stickman_rig_id)

