class_name StudioButtons
extends RefCounted

static func style_primary_button(btn: Button, bg_hex: String = "#3B82F6") -> void:
	var sb := StyleBoxFlat.new()
	sb.bg_color = Color.html(bg_hex)
	sb.set_corner_radius_all(8)
	sb.content_margin_left = 14
	sb.content_margin_right = 14
	sb.content_margin_top = 8
	sb.content_margin_bottom = 8
	btn.add_theme_stylebox_override("normal", sb)

