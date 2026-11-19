class_name StudioDialogs
extends RefCounted

static func create_confirm_dialog(title_text: String, body_text: String) -> ConfirmationDialog:
	var dlg := ConfirmationDialog.new()
	dlg.title = title_text
	dlg.dialog_text = body_text
	return dlg

