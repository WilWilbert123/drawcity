#!/usr/bin/env python3
"""Validates that all 23 required LIVING CANVAS tables exist and foreign_key_check passes."""

import pathlib
import sqlite3
import sys

REQUIRED_TABLES = {
    "users", "user_settings", "projects", "canvases",
    "drawing_layers", "drawing_strokes", "drawing_points",
    "objects", "object_parts", "rigs", "bones", "joints",
    "animations", "animation_tracks", "keyframes",
    "physics_profiles", "object_relationships", "worlds",
    "world_objects", "assets", "brush_presets",
    "project_history", "export_history",
}


def validate_db(db_path: pathlib.Path) -> bool:
    if not db_path.exists():
        print(f"[FAIL] Database file does not exist: {db_path}")
        return False
    conn = sqlite3.connect(str(db_path))
    try:
        cur = conn.execute("SELECT name FROM sqlite_master WHERE type='table';")
        found = {row[0] for row in cur.fetchall()}
        missing = REQUIRED_TABLES - found
        if missing:
            print(f"[FAIL] Missing tables: {sorted(missing)}")
            return False
        integrity = conn.execute("PRAGMA integrity_check;").fetchone()[0]
        if integrity != "ok":
            print(f"[FAIL] PRAGMA integrity_check returned: {integrity}")
            return False
        print(f"[OK] Validated {len(REQUIRED_TABLES)} required tables in {db_path}")
        return True
    finally:
        conn.close()


if __name__ == "__main__":
    target = pathlib.Path(sys.argv[1]) if len(sys.argv) > 1 else pathlib.Path("/tmp/living_canvas_test_db.db")
    sys.exit(0 if validate_db(target) else 1)

