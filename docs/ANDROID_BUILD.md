# LIVING CANVAS — Android NDK & APK Build Guide

## Prerequisites Verified on Workspace
- **Android SDK**: `/Users/wilbertgamis/Library/Android/sdk`
- **Android NDK**: `/Users/wilbertgamis/Library/Android/sdk/ndk/26.1.10909125`
- **CMake & Ninja**: `/Users/wilbertgamis/Library/Android/sdk/cmake/3.22.1/bin`
- **Godot Engine**: `4.7.2.stable.official`

## Building the Native Android `arm64-v8a` GDExtension Library
```bash
/Users/wilbertgamis/Library/Android/sdk/cmake/3.22.1/bin/cmake --preset android-arm64-debug
/Users/wilbertgamis/Library/Android/sdk/cmake/3.22.1/bin/cmake --build --preset android-arm64-debug
```
Output binary: `godot_extension/bin/libliving_canvas_gdext.android.debug.arm64.so`

