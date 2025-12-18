#Requires -Version 5.1
<#!
Configures and builds the SDL app using CMake presets.
Defaults to static linking via vcpkg's x64-windows-static.

Usage examples:
  .\scripts\build.ps1                   # static preset
  .\scripts\build.ps1 -Preset dynamic   # dynamic preset
  .\scripts\build.ps1 -Run              # build then run
  .\scripts\build.ps1 -Clean            # remove build dir for chosen preset
!#>

[CmdletBinding()] param(
  [ValidateSet("static","dynamic")] [string]$Preset = "static",
  [switch]$Run,
  [switch]$RunVm,
  [switch]$Clean,
  [string]$VcpkgRoot = $env:VCPKG_ROOT
)

function Write-Info($msg) { Write-Host "[info] $msg" -ForegroundColor Cyan }
function Write-Warn($msg) { Write-Host "[warn] $msg" -ForegroundColor Yellow }
function Write-Err($msg)  { Write-Host "[err]  $msg" -ForegroundColor Red }
function Test-Command($name) {
  return [bool](Get-Command $name -ErrorAction SilentlyContinue)
}

# Resolve repo root (this script lives in LangJam/scripts)
$RepoRoot = Split-Path $PSScriptRoot -Parent
Push-Location $RepoRoot
try {
  if (-not (Test-Path $RepoRoot)) { Write-Err "Repo root not found: $RepoRoot"; exit 1 }

  if (-not $VcpkgRoot -or $VcpkgRoot.Trim() -eq "") { $VcpkgRoot = "C:\vcpkg" }
  $env:VCPKG_ROOT = $VcpkgRoot
  Write-Info "Using VCPKG_ROOT=$VcpkgRoot"

  $hasNinja = Test-Command ninja
  $hasCl = Test-Command cl
  if ($hasNinja -and -not $hasCl) {
    Write-Warn "Ninja detected but MSVC compiler (cl.exe) not on PATH. Falling back to Visual Studio generator preset."
    $hasNinja = $false
  }
  if ($Preset -eq "static") {
    $configurePreset = if ($hasNinja) { "windows-vcpkg-static" } else { "windows-vcpkg-static-msvc" }
  } else {
    $configurePreset = if ($hasNinja) { "windows-vcpkg" } else { "windows-vcpkg-msvc" }
  }
  $buildDir = Join-Path $RepoRoot ("build/" + $configurePreset)
  if ($configurePreset -like "*-msvc") {
    $exePath = Join-Path (Join-Path $buildDir "Debug") "LangJamSDL.exe"
    $vmExePath = Join-Path (Join-Path $buildDir "Debug") "LangJamVM.exe"
  } else {
    $exePath = Join-Path $buildDir "LangJamSDL.exe"
    $vmExePath = Join-Path $buildDir "LangJamVM.exe"
  }

  if ($Clean) {
    if (Test-Path $buildDir) {
      Write-Info "Cleaning build directory: $buildDir"
      Remove-Item -Recurse -Force $buildDir
    } else { Write-Info "No build directory to clean: $buildDir" }
  }

  if (-not (Test-Command cmake)) { Write-Err "cmake command not found. Run scripts/setup.ps1 first."; exit 1 }

  Write-Info "Configuring preset: $configurePreset"
  cmake --preset $configurePreset
  if ($LASTEXITCODE -ne 0) { Write-Err "CMake configure failed."; exit $LASTEXITCODE }

  Write-Info "Building preset: $configurePreset"
  cmake --build --preset $configurePreset
  if ($LASTEXITCODE -ne 0) { Write-Err "CMake build failed."; exit $LASTEXITCODE }

  if ($Run -and -not $RunVm) {
    if (Test-Path $exePath) {
      Write-Info "Running: $exePath"
      & $exePath
    } else {
      Write-Warn "Executable not found at $exePath"
    }
  } elseif ($RunVm) {
    if (Test-Path $vmExePath) {
      Write-Info "Running VM demo: $vmExePath"
      & $vmExePath
    } else {
      Write-Warn "VM executable not found at $vmExePath"
    }
  } else {
    Write-Host "\nBuild complete." -ForegroundColor Green
    Write-Host "SDL Binary: $exePath"
    Write-Host "VM Binary:  $vmExePath"
  }
}
finally {
  Pop-Location
}
