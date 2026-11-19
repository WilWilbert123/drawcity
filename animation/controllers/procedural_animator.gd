class_name ProceduralAnimatorGD
extends RefCounted

static func compute_wave_offset(time_sec: float, amplitude: float = 6.0, frequency: float = 2.5) -> float:
	return sin(time_sec * TAU * frequency) * amplitude

