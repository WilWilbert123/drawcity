-- LIVING CANVAS (Draw City) — Complete 23-Table SQLite 3 Schema (v8)
PRAGMA foreign_keys = ON;
PRAGMA journal_mode = WAL;

CREATE TABLE IF NOT EXISTS schema_migrations (
    version INTEGER PRIMARY KEY,
    applied_at_ms INTEGER NOT NULL,
    description TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS users (
    user_id INTEGER PRIMARY KEY AUTOINCREMENT,
    display_name TEXT NOT NULL,
    avatar_key TEXT NOT NULL DEFAULT 'artist_01',
    created_at_ms INTEGER NOT NULL,
    last_active_at_ms INTEGER NOT NULL
);

CREATE TABLE IF NOT EXISTS user_settings (
    setting_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    setting_key TEXT NOT NULL,
    setting_value TEXT NOT NULL,
    updated_at_ms INTEGER NOT NULL,
    UNIQUE(user_id, setting_key),
    FOREIGN KEY(user_id) REFERENCES users(user_id) ON DELETE CASCADE
);

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

CREATE TABLE IF NOT EXISTS objects (
    object_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    category TEXT NOT NULL DEFAULT 'other',
    classification_confidence REAL NOT NULL DEFAULT 1.0,
    pos_x REAL NOT NULL DEFAULT 0.0,
    pos_y REAL NOT NULL DEFAULT 0.0,
    rotation_rad REAL NOT NULL DEFAULT 0.0,
    scale_x REAL NOT NULL DEFAULT 1.0,
    scale_y REAL NOT NULL DEFAULT 1.0,
    created_at_ms INTEGER NOT NULL,
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS object_parts (
    part_id INTEGER PRIMARY KEY AUTOINCREMENT,
    object_id INTEGER NOT NULL,
    part_name TEXT NOT NULL,
    pivot_x REAL NOT NULL DEFAULT 0.0,
    pivot_y REAL NOT NULL DEFAULT 0.0,
    z_order INTEGER NOT NULL DEFAULT 0,
    FOREIGN KEY(object_id) REFERENCES objects(object_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS rigs (
    rig_id INTEGER PRIMARY KEY AUTOINCREMENT,
    object_id INTEGER NOT NULL,
    rig_type TEXT NOT NULL DEFAULT 'stickman',
    created_at_ms INTEGER NOT NULL,
    FOREIGN KEY(object_id) REFERENCES objects(object_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS bones (
    bone_id INTEGER PRIMARY KEY AUTOINCREMENT,
    rig_id INTEGER NOT NULL,
    parent_bone_id INTEGER DEFAULT NULL,
    name TEXT NOT NULL,
    start_joint_id INTEGER NOT NULL,
    end_joint_id INTEGER NOT NULL,
    length REAL NOT NULL DEFAULT 30.0,
    local_rotation_deg REAL NOT NULL DEFAULT 0.0,
    bound_part_id INTEGER DEFAULT NULL,
    FOREIGN KEY(rig_id) REFERENCES rigs(rig_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS joints (
    joint_id INTEGER PRIMARY KEY AUTOINCREMENT,
    rig_id INTEGER NOT NULL,
    name TEXT NOT NULL,
    pos_x REAL NOT NULL,
    pos_y REAL NOT NULL,
    rest_x REAL NOT NULL,
    rest_y REAL NOT NULL,
    min_angle_deg REAL NOT NULL DEFAULT -180.0,
    max_angle_deg REAL NOT NULL DEFAULT 180.0,
    FOREIGN KEY(rig_id) REFERENCES rigs(rig_id) ON DELETE CASCADE
);

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

CREATE TABLE IF NOT EXISTS physics_profiles (
    profile_id INTEGER PRIMARY KEY AUTOINCREMENT,
    object_id INTEGER NOT NULL,
    body_type TEXT NOT NULL DEFAULT 'dynamic',
    mass REAL NOT NULL DEFAULT 1.0,
    gravity_scale REAL NOT NULL DEFAULT 1.0,
    friction REAL NOT NULL DEFAULT 0.8,
    restitution REAL NOT NULL DEFAULT 0.15,
    buoyancy REAL NOT NULL DEFAULT 0.0,
    FOREIGN KEY(object_id) REFERENCES objects(object_id) ON DELETE CASCADE
);

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

CREATE TABLE IF NOT EXISTS project_history (
    history_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    action_label TEXT NOT NULL,
    snapshot_json TEXT NOT NULL,
    created_at_ms INTEGER NOT NULL,
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS export_history (
    export_id INTEGER PRIMARY KEY AUTOINCREMENT,
    project_id INTEGER NOT NULL,
    export_format TEXT NOT NULL,
    output_rel_path TEXT NOT NULL,
    exported_at_ms INTEGER NOT NULL,
    FOREIGN KEY(project_id) REFERENCES projects(project_id) ON DELETE CASCADE
);

