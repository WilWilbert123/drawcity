# LIVING CANVAS — Android NDK & APK Build Guide

## Prerequisites Verified on Workspace
- **Android SDK**: `/Users/wilbertgamis/Library/Android/sdk`
- **Android NDK**: `/Users/wilbertgamis/Library/Android/sdk/ndk/26.1.10909125`
- **Android Build-Tools**: `/Users/wilbertgamis/Library/Android/sdk/build-tools/36.0.0/apksigner`
- **CMake & Ninja**: `/Users/wilbertgamis/Library/Android/sdk/cmake/3.22.1/bin`
- **Godot Engine**: `4.7.2.stable.official`
- **Godot 4.7.2 Android Export Templates**: `~/Library/Application Support/Godot/export_templates/4.7.2.stable/android_debug.apk` & `android_release.apk`

## 1. Building the Native Android `arm64-v8a` GDExtension Library
```bash
/Users/wilbertgamis/Library/Android/sdk/cmake/3.22.1/bin/cmake --preset android-arm64-debug
/Users/wilbertgamis/Library/Android/sdk/cmake/3.22.1/bin/cmake --build --preset android-arm64-debug
```
Output binary: `godot_extension/bin/libliving_canvas_gdext.android.debug.arm64.so`

## 2. Packaging & Signing the Android APK
```bash
/opt/homebrew/bin/godot --headless --export-debug "Android" build/android/living_canvas.apk
```
Output signed APK: `build/android/living_canvas.apk` (bundles `lib/arm64-v8a/libliving_canvas_gdext.android.debug.arm64.so` with embedded SQLite 3 and `libgodot_android.so`).


