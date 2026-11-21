-- Migration 003: Projects, Canvases, and Project History
CREATE TABLE IF NOT EXISTS projects (
    project_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    title TEXT NOT NULL,
    description TEXT NOT NULL DEFAULT '',
    thumbnail_rel_path TEXT NOT NULL DEFAULT '',
    project_folder_rel_path TEXT NOT NULL DEFAULT '',
    created_at_ms INTEGER NOT NULL,
    updated_at_ms INTEGER NOT NULL,
    FOREIGN KEY(user_id) REFERENCES users(user_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS canvases (
    canvas_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    width INTEGER NOT NULL DEFAULT 1920,
    height INTEGER NOT NULL DEFAULT 1080,
    background_color TEXT NOT NULL DEFAULT '#181B24',
    zoom_level REAL NOT NULL DEFAULT 1.0,
    pan_x REAL NOT NULL DEFAULT 0.0,
    pan_y REAL NOT NULL DEFAULT 0.0,
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS project_history (
    history_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    action_label TEXT NOT NULL,
    snapshot_json TEXT NOT NULL,
    created_at_ms INTEGER NOT NULL,
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE
);
INSERT OR IGNORE INTO schema_migrations(version, applied_at_ms, description)
VALUES (3, strftime('%s','now') * 1000, '003_projects');

