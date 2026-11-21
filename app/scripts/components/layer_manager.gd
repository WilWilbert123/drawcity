class_name LayerManagerComponent
extends RefCounted

var active_layer_id: int = 1

func add_layer(layer_name: String) -> int:
	if AppState.drawing_bridge != null:
		active_layer_id = AppState.drawing_bridge.create_layer(layer_name)
	return active_layer_id

