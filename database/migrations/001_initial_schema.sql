-- Migration 001: Schema version tracking table
CREATE TABLE IF NOT EXISTS schema_migrations (
    version INTEGER PRIMARY KEY,
    applied_at_ms INTEGER NOT NULL,
    description TEXT NOT NULL
);
INSERT OR IGNORE INTO schema_migrations(version, applied_at_ms, description)
VALUES (1, strftime('%s','now') * 1000, '001_initial_schema');

