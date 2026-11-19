class_name DrawingAnalyzerGD
extends RefCounted

var classifier := ObjectClassifierGD.new()

func analyze_and_suggest_parts() -> Dictionary:
	return classifier.classify_active_drawing()

