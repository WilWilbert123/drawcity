class_name WorldCameraGD
extends Camera2D

func pan_camera(delta: Vector2) -> void:
	position += delta

func set_zoom_factor(z: float) -> void:
	var clamped := clampf(z, 0.25, 4.0)
	zoom = Vector2(clamped, clamped)

