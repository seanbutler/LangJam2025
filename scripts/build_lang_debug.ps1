
# Navigate to the build directory
$buildDir = Join-Path $PSScriptRoot "..\build"
if (-not (Test-Path $buildDir)) {
	New-Item -ItemType Directory -Path $buildDir | Out-Null
}
Set-Location $buildDir

# Configure the project (if not already configured)
cmake .. -G "Visual Studio 17 2022" -A x64

# Build target lang in Debug mode
cmake --build . --target lang --config Debug

