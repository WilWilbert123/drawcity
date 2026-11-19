-- Migration 002: Local User Profiles & User Settings
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
INSERT OR IGNORE INTO schema_migrations(version, applied_at_ms, description)
VALUES (2, strftime('%s','now') * 1000, '002_user_profiles');

