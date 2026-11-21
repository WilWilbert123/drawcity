class_name CanvasRenderer
extends Node2D

var show_grid: bool = true
var show_rig_overlay: bool = true
var city_time_sec: float = 0.0

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
	# City Street Ground Line (Emerald) & Water Surface Line (Cyan)
	draw_line(Vector2(0, 620), Vector2(1920, 620), Color(0.063, 0.725, 0.506, 0.45), 2.5)
	draw_line(Vector2(0, 740), Vector2(1920, 740), Color(0.024, 0.714, 0.831, 0.25), 1.5)

func _compute_stroke_deformations() -> Dictionary:
	var stroke_transforms := {}
	var stickman_offset := Vector2.ZERO
	var car_offset := Vector2.ZERO

	if AppState.physics_bridge != null:
		var car_st := AppState.physics_bridge.get_body_state(AppState.active_car_object_id)
		var car_raw_x := float(car_st.get("x", 0.0))
		var car_wrapped_x := fposmod(car_raw_x, 900.0) if absf(car_raw_x) > 0.01 else 0.0
		car_offset = Vector2(car_wrapped_x, 0.0)

		if AppState.stickman_in_car:
			stickman_offset = car_offset
		else:
			var char_st := AppState.physics_bridge.get_body_state(AppState.active_stickman_object_id)
			var char_raw_x := float(char_st.get("x", 0.0))
			var char_wrapped_x := fposmod(char_raw_x, 900.0) if absf(char_raw_x) > 0.01 else 0.0
			var char_raw_y := float(char_st.get("y", 0.0))
			var char_vert := clampf(char_raw_y - 480.0, -180.0, 120.0) if char_raw_y > 1.0 else 0.0
			stickman_offset = Vector2(char_wrapped_x, char_vert)

	if AppState.animation_bridge == null or AppState.active_stickman_rig_id <= 0:
		return {
			"stickman_offset": stickman_offset,
			"car_offset": car_offset,
			"bones_by_stroke": stroke_transforms
		}

	var parsed = JSON.parse_string(AppState.animation_bridge.serialize_rig_json(AppState.active_stickman_rig_id))
	if typeof(parsed) != TYPE_DICTIONARY:
		return {
			"stickman_offset": stickman_offset,
			"car_offset": car_offset,
			"bones_by_stroke": stroke_transforms
		}

	var joints_by_id := {}
	for j in parsed.get("joints", []):
		var jid := int(j.get("id", 0))
		joints_by_id[jid] = {
			"curr": Vector2(float(j.get("x", 0.0)), float(j.get("y", 0.0))),
			"rest": Vector2(float(j.get("rest_x", 0.0)), float(j.get("rest_y", 0.0)))
		}

	for b in parsed.get("bones", []):
		var sj := int(b.get("start_joint_id", 0))
		var ej := int(b.get("end_joint_id", 0))
		if not (joints_by_id.has(sj) and joints_by_id.has(ej)):
			continue
		var s_rest: Vector2 = joints_by_id[sj]["rest"]
		var e_rest: Vector2 = joints_by_id[ej]["rest"]
		var s_curr: Vector2 = joints_by_id[sj]["curr"]
		var e_curr: Vector2 = joints_by_id[ej]["curr"]
		var rest_angle := (e_rest - s_rest).angle()
		var curr_angle := (e_curr - s_curr).angle()
		var delta_angle := curr_angle - rest_angle
		for sid in b.get("bound_stroke_ids", []):
			stroke_transforms[int(sid)] = {
				"pivot_rest": s_rest,
				"pivot_curr": s_curr,
				"delta_angle": delta_angle
			}
	return {
		"stickman_offset": stickman_offset,
		"car_offset": car_offset,
		"bones_by_stroke": stroke_transforms
	}

func _get_role_offset(part_id: int, object_id: int, has_bone: bool, stickman_offset: Vector2, car_offset: Vector2) -> Vector2:
	# Buildings (10) and Trees (11) stay anchored to the city ground!
	if part_id == 10 or part_id == 11:
		return Vector2.ZERO
	# Clouds (3) drift gently across the sky
	if part_id == 3:
		return Vector2(fposmod(city_time_sec * 22.0, 960.0), 0.0)
	# Birds (4) fly & bob in the sky
	if part_id == 4:
		return Vector2(fposmod(city_time_sec * 55.0, 960.0), sin(city_time_sec * 4.5) * 14.0)
	# Fish & Water (5) swim with wave motion
	if part_id == 5:
		return Vector2(sin(city_time_sec * 2.2) * 35.0, cos(city_time_sec * 3.0) * 6.0)
	# Car (part_id == 2 or object_id == 2) moves with car physics only
	if part_id == 2 or object_id == 2:
		return car_offset
	# Stickman citizen (part_id == 1 or bound to rig bone) moves with stickman offset
	if part_id == 1 or object_id == 1 or has_bone:
		return stickman_offset
	return Vector2.ZERO

func _draw_preserved_strokes() -> void:
	if AppState.drawing_bridge == null:
		return
	var json_str := AppState.drawing_bridge.serialize_canvas_json()
	var parsed = JSON.parse_string(json_str)
	if typeof(parsed) != TYPE_DICTIONARY or not parsed.has("strokes"):
		return
	var deform := _compute_stroke_deformations()
	var stickman_offset: Vector2 = deform["stickman_offset"]
	var car_offset: Vector2 = deform["car_offset"]
	var bones_by_stroke: Dictionary = deform["bones_by_stroke"]

	for s in parsed["strokes"]:
		if not s.get("visible", true):
			continue
		var sid := int(s.get("id", 0))
		var obj_id := int(s.get("object_id", 0))
		var part_id := int(s.get("part_id", 0))
		var pts_raw: Array = s.get("points", [])
		if pts_raw.is_empty():
			continue
		var col := ColorUtils.from_hex(str(s.get("color", "#F9FAFB")))
		var width := float(s.get("width", 4.0))
		var has_bone := bones_by_stroke.has(sid)
		var stroke_offset := _get_role_offset(part_id, obj_id, has_bone, stickman_offset, car_offset)

		var pivot_rest := Vector2.ZERO
		var pivot_curr := Vector2.ZERO
		var delta_angle := 0.0
		if has_bone:
			pivot_rest = bones_by_stroke[sid]["pivot_rest"]
			pivot_curr = bones_by_stroke[sid]["pivot_curr"]
			delta_angle = bones_by_stroke[sid]["delta_angle"]

		if pts_raw.size() == 1:
			var pt := Vector2(float(pts_raw[0][0]), float(pts_raw[0][1]))
			if has_bone:
				pt = pivot_curr + (pt - pivot_rest).rotated(delta_angle)
			draw_circle(pt + stroke_offset, width * 0.5, col)
		else:
			var poly := PackedVector2Array()
			for raw_pt in pts_raw:
				var pt := Vector2(float(raw_pt[0]), float(raw_pt[1]))
				if has_bone:
					pt = pivot_curr + (pt - pivot_rest).rotated(delta_angle)
				poly.push_back(pt + stroke_offset)
			draw_polyline(poly, col, width, true)

func _draw_rig_skeleton(rig_id: int) -> void:
	if AppState.animation_bridge == null:
		return
	var parsed = JSON.parse_string(AppState.animation_bridge.serialize_rig_json(rig_id))
	if typeof(parsed) != TYPE_DICTIONARY:
		return
	var deform := _compute_stroke_deformations()
	var stickman_offset: Vector2 = deform["stickman_offset"]
	var joints_by_id := {}
	for j in parsed.get("joints", []):
		var jid := int(j.get("id", 0))
		var pos := Vector2(float(j.get("x", 0.0)), float(j.get("y", 0.0))) + stickman_offset
		joints_by_id[jid] = pos
	for b in parsed.get("bones", []):
		var sj := int(b.get("start_joint_id", 0))
		var ej := int(b.get("end_joint_id", 0))
		if joints_by_id.has(sj) and joints_by_id.has(ej):
			draw_line(joints_by_id[sj], joints_by_id[ej], Color(0.231, 0.510, 0.965, 0.85), 3.0, true)
	for pos in joints_by_id.values():
		draw_circle(pos, 5.5, Color.html("#F59E0B"))



