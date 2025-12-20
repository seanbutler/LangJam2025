
# LangJam2025


LangJam2025 is a modular C++ project for experimenting with language and VM design, rapid prototyping, and cross-platform SDL2 graphics.

## Structure
- `runtime/` — SDL2-based virtual machine demo
- `lang/` — Language front-end (tokenizer, parser, AST)
- `min/` — Minimal C++ hello world example
- `scripts/` — PowerShell build and helper scripts

## Prerequisites (Windows)
- Visual Studio Code 
- C++ compiler
- PowerShell (for scripts)
- CMake 
- vcpkg 

### Install vcpkg
```powershell
# Choose a location, e.g. C:\vcpkg
git clone https://github.com/microsoft/vcpkg C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat

# Make it available to CMake presets
$env:VCPKG_ROOT = "C:\vcpkg"
```


## Building & Running

Use the provided PowerShell scripts for easy setup and building:

```powershell
./scripts/build_runtime_debug.ps1
# Output: build/runtime/Debug/runtime.exe
```

Or build the minimal hello world:

```powershell
cd min
cmake -B build -S .
cmake --build build --config Debug
./build/Debug/min_hello.exe
```

The correct SDL2 DLL is automatically copied to your output directory by the build scripts.

For more details and future plans, see `ROADMAP.md`.
