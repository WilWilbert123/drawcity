extends Node

func can_undo() -> bool:
	return AppState.drawing_bridge != null and AppState.drawing_bridge.can_undo()

func can_redo() -> bool:
	return AppState.drawing_bridge != null and AppState.drawing_bridge.can_redo()

func perform_undo() -> String:
	if can_undo():
		return AppState.drawing_bridge.undo()
	return ""

func perform_redo() -> String:
	if can_redo():
		return AppState.drawing_bridge.redo()
	return ""

