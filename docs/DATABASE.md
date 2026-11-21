# LIVING CANVAS — SQLite 3 Database Architecture

- **Database Location**: `user://data/living_canvas.db`
- **Engine**: SQLite 3 C API (`sqlite3.h` / `sqlite3.c` amalgamation for Android NDK and system/Homebrew SQLite 3 on desktop)
- **Journal Mode**: `WAL` with `PRAGMA foreign_keys = ON;`
- **Schema Version**: `8` (23 required domain tables + `schema_migrations`)

## Tables
1. `users`
2. `user_settings`
3. `projects`
4. `canvases`
5. `drawing_layers`
6. `drawing_strokes` (stores binary `points_blob` of raw `StrokePoint` coordinates + pressure)
7. `drawing_points`
8. `objects`
9. `object_parts`
10. `rigs`
11. `bones`
12. `joints`
13. `animations`
14. `animation_tracks`
15. `keyframes`
16. `physics_profiles`
17. `object_relationships`
18. `worlds`
19. `world_objects`
20. `assets`
21. `brush_presets`
22. `project_history`
23. `export_history`

