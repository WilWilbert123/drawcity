-- Migration 004: Drawing Layers, Strokes, and Stroke Points
CREATE TABLE IF NOT EXISTS drawing_layers (
    layer_id INTEGER PRIMARY KEY AUTOINCREMENT,
    canvas_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    z_index INTEGER NOT NULL DEFAULT 0,
    opacity REAL NOT NULL DEFAULT 1.0,
    visible INTEGER NOT NULL DEFAULT 1,
    locked INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY(canvas_id) REFERENCES canvases(canvas_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS drawing_strokes (
    stroke_id INTEGER PRIMARY KEY AUTOINCREMENT,
    layer_id INTEGER NOT NULL,
    object_id INTEGER DEFAULT NULL,
    part_id INTEGER DEFAULT NULL,
    brush_type INTEGER NOT NULL DEFAULT 0,
    color_hex TEXT NOT NULL DEFAULT '#F9FAFB',
    base_width REAL NOT NULL DEFAULT 4.0,
    stroke_order INTEGER NOT NULL DEFAULT 0,
    visible INTEGER NOT NULL DEFAULT 1,
    locked INTEGER NOT NULL DEFAULT 0,
    points_blob BLOB,
    FOREIGN KEY(layer_id) REFERENCES drawing_layers(layer_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS drawing_points (
    point_id INTEGER PRIMARY KEY AUTOINCREMENT,
    stroke_id INTEGER NOT NULL,
    point_index INTEGER NOT NULL,
    x REAL NOT NULL,
    y REAL NOT NULL,
    pressure REAL NOT NULL DEFAULT 1.0,
    timestamp_ms INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY(stroke_id) REFERENCES drawing_strokes(stroke_id) ON DELETE CASCADE
);
INSERT OR IGNORE INTO schema_migrations(version, applied_at_ms, description)
VALUES (4, strftime('%s','now') * 1000, '004_drawing_layers');

