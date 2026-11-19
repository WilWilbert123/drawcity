-- Migration 006: Objects, Parts, Rigs, Bones, Joints, and Physics Profiles
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
INSERT OR IGNORE INTO schema_migrations(version, applied_at_ms, description)
VALUES (6, strftime('%s','now') * 1000, '006_objects_and_rigging');

