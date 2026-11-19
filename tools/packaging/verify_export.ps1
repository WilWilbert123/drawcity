$ErrorActionPreference = "Stop"
$ProjectRoot = Resolve-Path "$PSScriptRoot\..\.."
Set-Location $ProjectRoot
if (-not (Test-Path "godot_extension/bin/libliving_canvas_gdext.android.debug.arm64.so")) {
    throw "Android arm64 GDExtension shared library is missing."
}
Write-Host "[OK] Android arm64 GDExtension binary verified."

