class_name EnvironmentControllerGD
extends Node

var time_of_day_hours: float = 12.0
var wind_speed: float = 20.0
var raining: bool = false

func toggle_rain() -> bool:
	raining = not raining
	return raining

