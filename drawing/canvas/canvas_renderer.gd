class_name CanvasRenderer
extends Node2D

var show_grid: bool = true
var show_rig_overlay: bool = true

func _draw() -> void:
	if show_grid:
		_draw_studio_grid()
	_draw_preserved_strokes()
	if show_rig_overlay and AppState.active_stickman_rig_id > 0:
		_draw_rig_skeleton(AppState.active_stickman_rig_id)

func _draw_studio_grid() -> void:
	var grid_color := Color(1.0, 1.0, 1.0, 0.04)
	for x in range(0, 1920, 32):
		draw_line(Vector2(x, 0), Vector2(x, 1080), grid_color, 1.0)
	for y in range(0, 1080, 32):
		draw_line(Vector2(0, y), Vector2(1920, y), grid_color, 1.0)

func _draw_preserved_strokes() -> void:
	if AppState.drawing_bridge == null:
		return
	var json_str := AppState.drawing_bridge.serialize_canvas_json()
	var parsed = JSON.parse_string(json_str)
	if typeof(parsed) != TYPE_DICTIONARY or not parsed.has("strokes"):
		return
	for s in parsed["strokes"]:
		if not s.get("visible", true):
			continue
		var pts_raw: Array = s.get("points", [])
		if pts_raw.is_empty():
			continue
		var col := ColorUtils.from_hex(str(s.get("color", "#F9FAFB")))
		var width := float(s.get("width", 4.0))
		if pts_raw.size() == 1:
			draw_circle(Vector2(pts_raw[0][0], pts_raw[0][1]), width * 0.5, col)
		else:
			var poly := PackedVector2Array()
			for pt in pts_raw:
				poly.push_back(Vector2(float(pt[0]), float(pt[1])))
			draw_polyline(poly, col, width, true)

func _draw_rig_skeleton(rig_id: int) -> void:
	if AppState.animation_bridge == null:
		return
	var parsed = JSON.parse_string(AppState.animation_bridge.serialize_rig_json(rig_id))
	if typeof(parsed) != TYPE_DICTIONARY:
		return
	var joints_by_id := {}
	for j in parsed.get("joints", []):
		var jid := int(j.get("id", 0))
		var pos := Vector2(float(j.get("x", 0.0)), float(j.get("y", 0.0)))
		joints_by_id[jid] = pos
	for b in parsed.get("bones", []):
		var sj := int(b.get("start_joint_id", 0))
		var ej := int(b.get("end_joint_id", 0))
		if joints_by_id.has(sj) and joints_by_id.has(ej):
			draw_line(joints_by_id[sj], joints_by_id[ej], Color(0.231, 0.510, 0.965, 0.85), 3.0, true)
	for pos in joints_by_id.values():
		draw_circle(pos, 5.5, Color.html("#F59E0B"))

