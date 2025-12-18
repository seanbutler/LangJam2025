#Requires -Version 5.1
<#!
Sets up the C++ dev environment for this repo on Windows:
- Ensures vcpkg is installed and bootstrapped
- Sets VCPKG_ROOT for current and user session
- Optionally installs Ninja and CMake with winget
- Verifies basic compiler tools availability

Usage examples:
  .\scripts\setup.ps1            # default, vcpkg at C:\vcpkg
  .\scripts\setup.ps1 -VcpkgRoot "D:\dev\vcpkg"
  .\scripts\setup.ps1 -SkipNinja -SkipCMake
!#>

[CmdletBinding()] param(
  [string]$VcpkgRoot = $env:VCPKG_ROOT,
  [switch]$SkipNinja,
  [switch]$SkipCMake
)

function Write-Info($msg) { Write-Host "[info] $msg" -ForegroundColor Cyan }
function Write-Warn($msg) { Write-Host "[warn] $msg" -ForegroundColor Yellow }
function Write-Err($msg)  { Write-Host "[err]  $msg" -ForegroundColor Red }

function Test-Command($name) {
  return [bool](Get-Command $name -ErrorAction SilentlyContinue)
}

function Install-WingetPackage {
  param([string[]]$Ids)
  if (-not (Test-Command winget)) { Write-Warn "winget not available; skipping install for $Ids"; return $false }
  foreach ($id in $Ids) {
    Write-Info "Installing $id via winget..."
    try {
      winget install --id $id -e --source winget -h --accept-package-agreements --accept-source-agreements | Out-Null
      Start-Sleep -Seconds 2
      return $true
    } catch {
      Write-Warn "Failed to install $id via winget: $_"
    }
  }
  return $false
}

# Determine VCPKG_ROOT
if (-not $VcpkgRoot -or $VcpkgRoot.Trim() -eq "") { $VcpkgRoot = "C:\vcpkg" }
Write-Info "Using VCPKG_ROOT=$VcpkgRoot"

# Ensure Git
if (-not (Test-Command git)) {
  Write-Warn "git not found. Attempting install via winget (Git.Git)."
  $ok = Install-WingetPackage -Ids @("Git.Git")
  if (-not $ok) { Write-Err "git is required. Please install Git and re-run."; exit 1 }
}

# Install/Bootstrap vcpkg
if (-not (Test-Path $VcpkgRoot)) {
  Write-Info "Cloning vcpkg into $VcpkgRoot"
  try {
    git clone https://github.com/microsoft/vcpkg $VcpkgRoot
  } catch {
    Write-Err "Failed to clone vcpkg: $_"; exit 1
  }
}

# Bootstrap
if (-not (Test-Path (Join-Path $VcpkgRoot "vcpkg.exe"))) {
  Write-Info "Bootstrapping vcpkg..."
  $bootstrap = Join-Path $VcpkgRoot "bootstrap-vcpkg.bat"
  if (-not (Test-Path $bootstrap)) { Write-Err "Missing bootstrap script at $bootstrap"; exit 1 }
  & $bootstrap
}

# Set environment variable for current session and persist for user
Write-Info "Setting VCPKG_ROOT in current and user environment"
$env:VCPKG_ROOT = $VcpkgRoot
[Environment]::SetEnvironmentVariable("VCPKG_ROOT", $VcpkgRoot, "User")

# Ensure Ninja
if (-not $SkipNinja) {
  if (-not (Test-Command ninja)) {
    Write-Warn "ninja not found. Attempting install via winget (Ninja-build.Ninja)."
    $ok = Install-WingetPackage -Ids @("Ninja-build.Ninja", "Ninja-build")
    if (-not $ok) { Write-Warn "Could not install ninja automatically. You can install it via winget or from https://ninja-build.org/" }
  } else {
    Write-Info "ninja found."
  }
}

# Ensure CMake
if (-not $SkipCMake) {
  if (-not (Test-Command cmake)) {
    Write-Warn "cmake not found. Attempting install via winget (Kitware.CMake)."
    $ok = Install-WingetPackage -Ids @("Kitware.CMake")
    if (-not $ok) { Write-Warn "Could not install CMake automatically. Please install and re-run." }
  } else {
    Write-Info "cmake found."
  }
}

# Quick compiler check (MSVC cl.exe preferred)
$hasCl = Test-Command cl
$hasCxx = Test-Command cxx
$hasGpp = Test-Command g++
if (-not $hasCl -and -not $hasGpp) {
  Write-Warn "No compiler detected on PATH (cl.exe or g++). If using MSVC, run from 'Developer PowerShell for VS' or ensure Build Tools are installed."
}

Write-Host "\nSetup complete." -ForegroundColor Green
Write-Host "VCPKG_ROOT: $env:VCPKG_ROOT"
if (Test-Command cmake) { cmake --version }
if (Test-Command ninja) { ninja --version }
