extends Node

var theme_name: String = "dark_theme"
var target_fps: int = 60
var autosave_enabled: bool = true

func save_setting(key: String, value: String) -> bool:
	if not DatabaseManager.is_ready:
		return false
	return DatabaseManager.storage_bridge.set_user_setting(AppState.current_user_id, key, value)

func load_setting(key: String, fallback: String = "") -> String:
	if not DatabaseManager.is_ready:
		return fallback
	return DatabaseManager.storage_bridge.get_user_setting(AppState.current_user_id, key, fallback)

