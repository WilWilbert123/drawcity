class_name StudioButtons
extends RefCounted

static func make_flat_box(bg_hex: String, border_hex: String = "", radius: int = 8, pad_h: int = 14, pad_v: int = 7) -> StyleBoxFlat:
	var sb := StyleBoxFlat.new()
	sb.bg_color = Color.html(bg_hex)
	sb.set_corner_radius_all(radius)
	if border_hex != "":
		sb.border_color = Color.html(border_hex)
		sb.set_border_width_all(1)
	sb.content_margin_left = pad_h
	sb.content_margin_right = pad_h
	sb.content_margin_top = pad_v
	sb.content_margin_bottom = pad_v
	return sb

static func style_primary_button(btn: Button, bg_hex: String = "#3B82F6") -> void:
	btn.add_theme_stylebox_override("normal", make_flat_box(bg_hex, "#60A5FA", 8, 14, 7))
	btn.add_theme_stylebox_override("hover", make_flat_box("#2563EB", "#93C5FD", 8, 14, 7))
	btn.add_theme_stylebox_override("pressed", make_flat_box("#1D4ED8", "#93C5FD", 8, 14, 7))
	btn.add_theme_color_override("font_color", Color.html("#F9FAFB"))

static func style_secondary_button(btn: Button, is_active: bool = false) -> void:
	if is_active:
		style_primary_button(btn, "#2563EB")
	else:
		btn.add_theme_stylebox_override("normal", make_flat_box("#1E2433", "#2E364D", 8, 12, 6))
		btn.add_theme_stylebox_override("hover", make_flat_box("#283044", "#3B82F6", 8, 12, 6))
		btn.add_theme_stylebox_override("pressed", make_flat_box("#1D4ED8", "#60A5FA", 8, 12, 6))
		btn.add_theme_color_override("font_color", Color.html("#E5E7EB"))

static func style_accent_button(btn: Button, bg_hex: String, border_hex: String) -> void:
	btn.add_theme_stylebox_override("normal", make_flat_box(bg_hex, border_hex, 8, 13, 6))
	btn.add_theme_stylebox_override("hover", make_flat_box(border_hex, "#F9FAFB", 8, 13, 6))
	btn.add_theme_stylebox_override("pressed", make_flat_box(bg_hex, "#F9FAFB", 8, 13, 6))
	btn.add_theme_color_override("font_color", Color.html("#F9FAFB"))


