class_name FluidSimulationGD
extends RefCounted

var water_surface_y: float = 520.0

func is_submerged(point_y: float) -> bool:
	return point_y >= water_surface_y

