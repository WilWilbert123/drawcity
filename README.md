# LIVING CANVAS (Draw City)

**Draw It. Animate It. Bring It to Life.**

LIVING CANVAS (`drawcity`) is an offline-first interactive 2D drawing, skeletal rigging, animation, physics, object recognition, and world simulation application built with **C++17**, **Godot 4**, **GDExtension**, and **SQLite 3** for Android and Desktop.

## Core Design Principle

**Preserve the User's Original Drawing.**
Every stroke, point coordinate, pressure value, color, layer, and proportion drawn by the user is preserved as editable vector/stroke data in C++ and SQLite (`user://data/living_canvas.db`). Rigging, skeletal animation, physics simulation, vehicle driving, and world interactions deform and animate the user's actual artwork without replacing it with stock assets.

## Architecture Overview

- **`engine/`**: Modular C++17 core engine (`core`, `drawing`, `animation`, `rigging`, `physics`, `recognition`, `interaction`, `world`, `objects`, `storage`, `serialization`, `history`, `platform`) plus embedded SQLite 3 C API support for 100% offline desktop and Android NDK builds.
- **`godot_extension/`**: GDExtension bridge exposing the C++17 engine to Godot 4 (`LivingCanvasEngine`, `DrawingBridge`, `AnimationBridge`, `PhysicsBridge`, `RecognitionBridge`, `StorageBridge`).
- **`app/`**: Godot 4 scenes, UI controllers, autoload managers, dark studio themes, and asset manifests.
- **`database/`**: SQLite 3 schema, incremental migrations (`001` through `008`), seed data, and GDScript repository wrappers.
- **`config/`**: JSON configuration files for application, engine, drawing, animation, physics, recognition, and database subsystems.

## Building & Running

### 1. Build the C++17 Engine & GDExtension (macOS / Desktop)
```bash
/Users/wilbertgamis/Library/Android/sdk/cmake/3.22.1/bin/cmake --preset macos-debug
/Users/wilbertgamis/Library/Android/sdk/cmake/3.22.1/bin/cmake --build --preset macos-debug
```

### 2. Build for Android NDK (`arm64-v8a`)
```bash
/Users/wilbertgamis/Library/Android/sdk/cmake/3.22.1/bin/cmake --preset android-arm64-debug
/Users/wilbertgamis/Library/Android/sdk/cmake/3.22.1/bin/cmake --build --preset android-arm64-debug
```

### 3. Launch in Godot 4
```bash
/opt/homebrew/bin/godot --path .
```

# drawcity
