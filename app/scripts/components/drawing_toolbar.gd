class_name DrawingToolbarComponent
extends RefCounted

var active_tool: int = 0 # 0=Pencil, 1=Pen, 2=Marker, 3=SoftBrush, 4=Eraser

func select_tool(tool_index: int) -> void:
	active_tool = clampi(tool_index, 0, 4)
	if AppState.drawing_bridge != null:
		AppState.drawing_bridge.select_brush(active_tool)

