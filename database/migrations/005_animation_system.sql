-- Migration 005: Animations, Tracks, and Keyframes
CREATE TABLE IF NOT EXISTS animations (
    animation_id INTEGER PRIMARY KEY AUTOINCREMENT,
    object_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    duration_sec REAL NOT NULL DEFAULT 1.0,
    fps INTEGER NOT NULL DEFAULT 24,
    loop INTEGER NOT NULL DEFAULT 1,
    FOREIGN KEY(object_id) REFERENCES objects(object_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS animation_tracks (
    track_id INTEGER PRIMARY KEY AUTOINCREMENT,
    animation_id INTEGER NOT NULL,
    target_bone_name TEXT NOT NULL,
    property_type TEXT NOT NULL DEFAULT 'transform',
    FOREIGN KEY(animation_id) REFERENCES animations(animation_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS keyframes (
    keyframe_id INTEGER PRIMARY KEY AUTOINCREMENT,
    track_id INTEGER NOT NULL,
    time_sec REAL NOT NULL,
    offset_x REAL NOT NULL DEFAULT 0.0,
    offset_y REAL NOT NULL DEFAULT 0.0,
    rotation_deg REAL NOT NULL DEFAULT 0.0,
    scale_x REAL NOT NULL DEFAULT 1.0,
    scale_y REAL NOT NULL DEFAULT 1.0,
    easing INTEGER NOT NULL DEFAULT 1,
    FOREIGN KEY(track_id) REFERENCES animation_tracks(track_id) ON DELETE CASCADE
);
INSERT OR IGNORE INTO schema_migrations(version, applied_at_ms, description)
VALUES (5, strftime('%s','now') * 1000, '005_animation_system');

