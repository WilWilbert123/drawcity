class_name ColorUtils
extends RefCounted

static func to_hex(color: Color) -> String:
	return "#" + color.to_html(false).to_upper()

static func from_hex(hex_str: String, fallback: Color = Color.WHITE) -> Color:
	if Color.html_is_valid(hex_str):
		return Color.html(hex_str)
	return fallback

