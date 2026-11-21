class_name WaterNodeGD
extends Node2D

@export var water_level_y: float = 520.0
var wave_phase: float = 0.0

func _process(delta: float) -> void:
	wave_phase += delta * 2.0

