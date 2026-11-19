class_name ObjectRepositoryGD
extends RefCounted

var _objects_by_id: Dictionary = {}
var _next_id: int = 1

func register_object(name: String, category: String, stroke_ids: PackedInt64Array, pos: Vector2) -> Dictionary:
	var obj_id := _next_id
	_next_id += 1
	var entry := {
		"object_id": obj_id,
		"name": name,
		"category": category,
		"stroke_ids": stroke_ids,
		"position": pos,
		"rig_id": 0
	}
	_objects_by_id[obj_id] = entry
	return entry

func get_object(object_id: int) -> Dictionary:
	return _objects_by_id.get(object_id, {})

func list_objects() -> Array:
	return _objects_by_id.values()

