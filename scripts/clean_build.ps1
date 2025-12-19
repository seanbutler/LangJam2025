# Clean build and generated dependencies for LangJam2025
# This script deletes the build folder and all generated dependency folders

$ErrorActionPreference = 'Stop'

# Paths to clean
$root = Split-Path -Parent $MyInvocation.MyCommand.Definition
$buildPath = Join-Path $root '..\build'
$buildPath = Resolve-Path $buildPath | ForEach-Object { $_.Path }

# List of generated dependency folders inside build
$generatedDeps = @(
    'build\_deps',
    'build\fmt-build',
    'build\fmt-src',
    'build\fmt-subbuild',
    'build\sdl2-build',
    'build\sdl2-src',
    'build\sdl2-subbuild'
)

Write-Host "Cleaning build folder: $buildPath"
if (Test-Path $buildPath) {
    Remove-Item -Path $buildPath -Recurse -Force
    Write-Host "Deleted: $buildPath"
} else {
    Write-Host "Build folder not found: $buildPath"
}

foreach ($dep in $generatedDeps) {
    $depPath = Join-Path $root "..\$dep"
    $depPath = Resolve-Path $depPath -ErrorAction SilentlyContinue | ForEach-Object { $_.Path }
    if ($depPath) {
        Write-Host "Cleaning dependency folder: $depPath"
        Remove-Item -Path $depPath -Recurse -Force
        Write-Host "Deleted: $depPath"
    } else {
        Write-Host "Dependency folder not found: $dep"
    }
}

Write-Host "Clean complete."
