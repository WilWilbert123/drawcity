#!/usr/bin/env python3
"""Initializes the LIVING CANVAS SQLite 3 database using database/schema.sql and database/seed.sql."""

import pathlib
import sqlite3
import sys


def initialize_db(db_path: pathlib.Path, project_root: pathlib.Path) -> None:
    db_path.parent.mkdir(parents=True, exist_ok=True)
    schema_sql = (project_root / "database" / "schema.sql").read_text(encoding="utf-8")
    seed_sql = (project_root / "database" / "seed.sql").read_text(encoding="utf-8")
    conn = sqlite3.connect(str(db_path))
    try:
        conn.executescript(schema_sql)
        conn.executescript(seed_sql)
        conn.commit()
    finally:
        conn.close()


if __name__ == "__main__":
    root = pathlib.Path(__file__).resolve().parents[2]
    target = pathlib.Path(sys.argv[1]) if len(sys.argv) > 1 else (root / "build" / "living_canvas_init.db")
    initialize_db(target, root)
    print(f"[OK] Initialized LIVING CANVAS SQLite database at {target}")

