# LIVING CANVAS — Rigging & Skeletal Animation Engine

- **`RigManager`, `Bone`, `Joint`**: Supports manual joint placement, bone hierarchy construction, two-bone analytical inverse kinematics (`solve_two_bone_ik`), and automatic 11-joint / 10-bone stickman rigging (`build_auto_stickman_rig`) that binds the user's original strokes to their nearest bones.
- **`AnimationEngine`, `AnimationClip`, `Keyframe`**: Evaluates `Linear`, `SmoothStep`, and `CubicInOut` keyframes across built-in presets (`idle`, `walk`, `run`, `jump`, `fall`, `fly`, `swim`, `attack`, `death`) and custom user tracks.

