-- Migration 008: Assets, Brush Presets, and Export History
CREATE TABLE IF NOT EXISTS assets (
    asset_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    asset_type TEXT NOT NULL,
    relative_path TEXT NOT NULL,
    byte_size INTEGER NOT NULL DEFAULT 0,
    checksum TEXT NOT NULL DEFAULT '',
    created_at_ms INTEGER NOT NULL,
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS brush_presets (
    preset_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    brush_type INTEGER NOT NULL,
    default_size REAL NOT NULL,
    min_size REAL NOT NULL,
    max_size REAL NOT NULL,
    opacity REAL NOT NULL,
    smoothing REAL NOT NULL,
    pressure_sensitive INTEGER NOT NULL DEFAULT 1
);

CREATE TABLE IF NOT EXISTS export_history (
    export_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    export_format TEXT NOT NULL,
    output_rel_path TEXT NOT NULL,
    exported_at_ms INTEGER NOT NULL,
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE
);
INSERT OR IGNORE INTO schema_migrations(version, applied_at_ms, description)
VALUES (8, strftime('%s','now') * 1000, '008_settings_and_metadata');

