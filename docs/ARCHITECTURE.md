# LIVING CANVAS — System Architecture

## 1. Layered Design
1. **C++17 Core Engine (`engine/`)**: Owns stroke geometry, brush interpolation, bone/joint hierarchies, two-bone IK solver, keyframe animation evaluation, custom locomotion physics, offline shape recognition, object relationships, world simulation, and SQLite 3 persistence.
2. **GDExtension Bridge (`godot_extension/`)**: Exposes `DrawingBridge`, `AnimationBridge`, `PhysicsBridge`, `RecognitionBridge`, and `StorageBridge` to Godot 4.
3. **Godot 4 & GDScript Application Layer (`app/`, `drawing/`, `animation/`, `physics/`, `recognition/`, `world/`, `objects/`, `database/`)**: Handles touch/stylus input, dark studio UI, scene tree coordination, and rendering of the user's original preserved strokes.

