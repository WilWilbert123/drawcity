# LIVING CANVAS — Development Roadmap & Phase Status

## Completed & Verified Phases
- **Phase 1 — Project Foundation**: C++17 engine, CMake presets, Godot 4.7.2 setup, GDExtension registration, macOS `arm64` `.dylib` build, and Android NDK `arm64-v8a` `.so` build verified.
- **Phase 2 — SQLite & Local Profiles**: All 23 required SQLite 3 tables (`v8`), incremental migrations `001`–`008`, WAL mode, foreign key enforcement, local user profile creation, user settings, and online backup API implemented and tested.
- **Phase 3 — Drawing Engine**: Preserve-original-artwork stroke engine, pressure sensitivity, smoothing, brush presets, eraser, layers, selection, transforms, undo/redo snapshots, and SQLite BLOB persistence implemented and tested.
- **Phase 4–11 Core Capabilities**: Object classification, 11-joint/10-bone auto stickman rigging, two-bone IK, animation clips (`idle`, `walk`, `run`, `jump`, `fly`, `swim`), ground/vehicle/aircraft/aquatic physics, and stickman-mounts-and-drives-drawn-car interaction implemented and verified via automated unit, integration, and performance tests.

## Future Enhancements
- **Android APK Packaging**: Requires installing Godot 4.7.2 Android export templates (`~/Library/Application Support/Godot/export_templates/4.7.2.stable/`) to package the compiled `libliving_canvas_gdext.android.debug.arm64.so` into a signed `.apk`.
- **Video/GIF Encoder**: Native MP4/GIF frame encoding in `ExportController` (currently exports full editable JSON bundles and PNG canvas snapshots).

