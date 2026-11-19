$ErrorActionPreference = "Stop"
$ProjectRoot = Resolve-Path "$PSScriptRoot\..\.."
Set-Location $ProjectRoot
New-Item -ItemType Directory -Force -Path "build/android" | Out-Null
godot --headless --export-debug "Android" "build/android/living_canvas.apk"

