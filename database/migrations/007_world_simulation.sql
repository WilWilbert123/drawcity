-- Migration 007: Worlds, World Objects, and Object Relationships
CREATE TABLE IF NOT EXISTS object_relationships (
    relationship_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    source_object_id INTEGER NOT NULL,
    target_object_id INTEGER NOT NULL,
    relationship_type TEXT NOT NULL,
    anchor_offset_x REAL NOT NULL DEFAULT 0.0,
    anchor_offset_y REAL NOT NULL DEFAULT 0.0,
    active INTEGER NOT NULL DEFAULT 1,
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE,
    FOREIGN KEY(source_object_id) REFERENCES objects(object_id) ON DELETE CASCADE,
    FOREIGN KEY(target_object_id) REFERENCES objects(object_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS worlds (
    world_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    name TEXT NOT NULL DEFAULT 'Main World',
    gravity_px_s2 REAL NOT NULL DEFAULT 980.0,
    wind_speed_px_s REAL NOT NULL DEFAULT 25.0,
    water_level_y REAL NOT NULL DEFAULT 720.0,
    ground_level_y REAL NOT NULL DEFAULT 880.0,
    time_of_day_hours REAL NOT NULL DEFAULT 12.0,
    weather_type TEXT NOT NULL DEFAULT 'clear',
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS world_objects (
    world_object_id INTEGER PRIMARY KEY AUTOINCREMENT,
    world_id INTEGER NOT NULL,
    object_id INTEGER NOT NULL,
    spawn_x REAL NOT NULL DEFAULT 0.0,
    spawn_y REAL NOT NULL DEFAULT 0.0,
    active INTEGER NOT NULL DEFAULT 1,
    FOREIGN KEY(world_id) REFERENCES worlds(world_id) ON DELETE CASCADE,
    FOREIGN KEY(object_id) REFERENCES objects(object_id) ON DELETE CASCADE
);
INSERT OR IGNORE INTO schema_migrations(version, applied_at_ms, description)
VALUES (7, strftime('%s','now') * 1000, '007_world_simulation');

