extends Node

var current_user_id: int = 1
var current_user_name: String = "Local Creator"
var current_project_id: int = 1
var current_project_title: String = "My Stickman & Car World"
var current_screen: String = "welcome"

var drawing_bridge: DrawingBridge
var animation_bridge: AnimationBridge
var physics_bridge: PhysicsBridge
var recognition_bridge: RecognitionBridge

var active_stickman_object_id: int = 1
var active_stickman_rig_id: int = 0
var active_car_object_id: int = 2
var stickman_in_car: bool = false

func _ready() -> void:
	drawing_bridge = DrawingBridge.new()
	animation_bridge = AnimationBridge.new()
	physics_bridge = PhysicsBridge.new()
	recognition_bridge = RecognitionBridge.new()
	physics_bridge.set_environment(980.0, 620.0, 520.0, 20.0)

