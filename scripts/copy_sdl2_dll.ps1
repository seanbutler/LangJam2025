# Copy the correct SDL2 DLL (release or debug) to the output directory after build
# Usage: .\copy_sdl2_dll.ps1 <Configuration> <TargetDir>
# Example: .\copy_sdl2_dll.ps1 Debug C:\Dev\LangJam2025\build\runtime\Debug

param(
    [string]$Configuration = "Debug",
    [string]$TargetDir = ""
)

# Path to SDL2 build output (adjust if needed)
$SDL2Root = ".\_deps\sdl2-build"

if ($Configuration -eq "Debug") {
    $dllName = "SDL2d.dll"
} else {
    $dllName = "SDL2.dll"
}

# Try to find the DLL in common locations
$dllPath = Join-Path $SDL2Root $dllName
if (-not (Test-Path $dllPath)) {
    # Try Debug or Release subfolders
    $dllPath = Join-Path $SDL2Root "$Configuration\$dllName"
}

if (-not (Test-Path $dllPath)) {
    Write-Host "Could not find $dllName in $SDL2Root or $dllPath or subfolders."
    exit 1
}

if (-not (Test-Path $TargetDir)) {
    Write-Host "Target directory $TargetDir does not exist."
    exit 1
}

Copy-Item $dllPath -Destination $TargetDir -Force
Write-Host "$dllName copied to $TargetDir"
