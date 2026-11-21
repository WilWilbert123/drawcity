extends Node

var sqlite_manager: SQLiteManager
var storage_bridge: StorageBridge
var is_ready: bool = false

func _ready() -> void:
	sqlite_manager = SQLiteManager.new()
	is_ready = sqlite_manager.open_default_database()
	storage_bridge = sqlite_manager.storage_bridge
	if is_ready:
		var profiles := storage_bridge.list_user_profiles()
		if profiles.is_empty():
			storage_bridge.create_user_profile("Local Creator", "artist_01")
		var projects := storage_bridge.list_projects(1)
		if projects.is_empty():
			storage_bridge.create_project(1, "My Stickman & Car World", "Default local project", 1920, 1080)

