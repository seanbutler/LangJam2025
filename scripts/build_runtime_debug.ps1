
# Navigate to the build directory
$buildDir = Join-Path $PSScriptRoot "..\build"
if (-not (Test-Path $buildDir)) {
	New-Item -ItemType Directory -Path $buildDir | Out-Null
}
Set-Location $buildDir

# Configure the project (if not already configured)
cmake .. -G "Visual Studio 17 2022" -A x64

# Build target runtime in Release mode

cmake --build . --target runtime --config Debug

# Copy the correct SDL2 DLL to the output directory
$outputDir = Join-Path $buildDir "runtime\\Debug"
$copyScript = Join-Path $PSScriptRoot "copy_sdl2_dll.ps1"
if (Test-Path $copyScript) {
	& $copyScript Debug $outputDir
} else {
	Write-Host "copy_sdl2_dll.ps1 not found in scripts directory."
}

