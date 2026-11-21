class_name ValidationUtils
extends RefCounted

static func is_valid_profile_name(name_text: String) -> bool:
	var trimmed := name_text.strip_edges()
	return trimmed.length() >= 2 and trimmed.length() <= 48

static func is_valid_project_title(title_text: String) -> bool:
	var trimmed := title_text.strip_edges()
	return trimmed.length() >= 1 and trimmed.length() <= 80

