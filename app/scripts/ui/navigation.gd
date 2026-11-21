class_name StudioNavigation
extends RefCounted

static func navigate_to(screen_name: String) -> void:
	AppState.current_screen = screen_name
	EventBus.screen_changed.emit(screen_name)

