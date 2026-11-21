class_name TimeUtils
extends RefCounted

static func format_seconds_mm_ss(seconds: float) -> String:
	var total := maxi(0, int(round(seconds)))
	var mins := total / 60
	var secs := total % 60
	return "%02d:%02d" % [mins, secs]

