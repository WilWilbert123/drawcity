# LIVING CANVAS — Synchronized File Structure

Every directory in the LIVING CANVAS repository contains real, non-empty implementation or configuration files. Zero empty directories and zero zero-byte files exist.

- Root configuration & build: `README.md`, `LICENSE`, `.gitignore`, `.gitattributes`, `CMakeLists.txt`, `CMakePresets.json`, `project.godot`, `export_presets.cfg`, `icon.svg`
- `config/`: 7 JSON subsystem configuration files (`app_config.json`, `engine_config.json`, `drawing_config.json`, `animation_config.json`, `physics_config.json`, `recognition_config.json`, `database_config.json`)
- `engine/include/living_canvas/` & `engine/src/`: 29 C++17 headers and 29 C++17 implementation files across `core`, `drawing`, `animation`, `rigging`, `physics`, `recognition`, `interaction`, `world`, `objects`, `storage`, `serialization`, `history`, and `platform`
- `godot_extension/`: `CMakeLists.txt`, `project.gdextension`, `register_types.hpp/.cpp`, and 5 GDExtension bridges (`drawing`, `animation`, `physics`, `recognition`, `storage`) + compiled `bin/` shared libraries for macOS `arm64` and Android `arm64-v8a`
- `app/`: Scenes (`main`, `authentication`, `home`, `editor`, `animation`, `objects`, `world`, `settings`, `dialogs`), scripts (`autoload`, `controllers`, `components`, `ui`, `utilities`), `assets`, `themes`, and `addons/living_canvas_bridge`
- `drawing/`, `animation/`, `physics/`, `recognition/`, `world/`, `objects/`: Domain scenes, scripts, and preset resources
- `database/`: `schema.sql`, `seed.sql`, `sqlite_manager.gd`, 8 SQL migration files (`001`–`008`), and 4 repository scripts
- `assets/`: 6 default asset manifests (`default_brushes`, `default_textures`, `example_projects`, `sample_animations`, `sound_effects`, `demo_worlds`)
- `tests/`: 6 unit test scripts, 4 integration test scripts, 2 performance test scripts, and 3 JSON test fixtures
- `tools/`: Build scripts (`build_linux.sh`, `build_windows.ps1`, `build_android.ps1`, `clean_build.ps1`), Python SQLite tools (`initialize_database.py`, `validate_database.py`), and packaging scripts (`package_android.ps1`, `verify_export.ps1`)
- `docs/`: 10 architecture and technical reference documents

