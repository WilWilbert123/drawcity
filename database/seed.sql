-- Default brush presets and schema version seed data
INSERT OR IGNORE INTO schema_migrations(version, applied_at_ms, description)
VALUES (8, strftime('%s','now') * 1000, 'Complete 23-table LIVING CANVAS offline schema v8');

INSERT OR IGNORE INTO brush_presets(name, brush_type, default_size, min_size, max_size, opacity, smoothing, pressure_sensitive)
VALUES
    ('Pencil', 0, 4.0, 1.0, 32.0, 1.0, 0.35, 1),
    ('Pen', 1, 5.0, 1.0, 48.0, 1.0, 0.50, 1),
    ('Marker', 2, 14.0, 4.0, 64.0, 0.78, 0.45, 0),
    ('Soft Brush', 3, 18.0, 4.0, 64.0, 0.65, 0.60, 1),
    ('Eraser', 4, 20.0, 2.0, 96.0, 1.0, 0.25, 0);

