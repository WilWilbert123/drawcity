class_name CloudNodeGD
extends Node2D

@export var drift_speed: float = 22.0

func _process(delta: float) -> void:
	position.x += drift_speed * delta
	if position.x > 2100.0:
		position.x = -180.0

