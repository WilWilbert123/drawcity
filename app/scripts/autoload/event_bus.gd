extends Node

signal screen_changed(screen_name: String)
signal active_profile_changed(user_id: int, display_name: String)
signal active_project_changed(project_id: int, title: String)
signal stroke_committed(stroke_id: int)
signal object_registered(object_id: int, category: String)
signal rig_updated(rig_id: int)
signal animation_state_changed(clip_name: String, is_playing: bool)
signal vehicle_mounted(character_id: int, vehicle_id: int)

