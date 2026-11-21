class_name DrawingLayerManagerGD
extends RefCounted

func create_layer(layer_name: String) -> int:
	if AppState.drawing_bridge != null:
		return AppState.drawing_bridge.create_layer(layer_name)
	return 1

