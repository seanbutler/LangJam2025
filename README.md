# LangJam SDL + VM via CMake

A minimal C/C++ SDL2 app scaffolded with CMake + vcpkg manifest mode, plus a small 32-bit cell-based VM that can be linked as a library.

## Prerequisites (Windows)
- C++ compiler: MSVC (Visual Studio Build Tools) / VS 2022
- CMake 3.21+
- vcpkg installed and bootstrapped
- (Optional) Ninja build system for faster builds

### Install vcpkg
```powershell
# Choose a location, e.g. C:\vcpkg
git clone https://github.com/microsoft/vcpkg C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat

# Make it available to CMake presets
$env:VCPKG_ROOT = "C:\vcpkg"
```

## Build (Presets)
This project uses CMake presets and vcpkg manifest mode.

If you want the easiest flow, use the PowerShell helper scripts:

```powershell
./scripts/setup.ps1
./scripts/build.ps1 -Run
```

### Static linking (recommended)
```powershell
# Configure with vcpkg static triplet
cmake --preset windows-vcpkg-static

# Build
cmake --build --preset windows-vcpkg-static

# Run the app
./build/windows-vcpkg-static/LangJamSDL.exe
```

### Dynamic linking (optional)
```powershell
cmake --preset windows-vcpkg
cmake --build --preset windows-vcpkg
./build/windows-vcpkg/LangJamSDL.exe
```
If Ninja isn't installed, either install it (`winget install Ninja-build`) or remove the generator in `CMakePresets.json` and let CMake choose MSVC.

Note: the Ninja presets also require MSVC's `cl.exe` to be available on your PATH (e.g. a Visual Studio Developer PowerShell). If it isn't, use the `*-msvc` presets or run `./scripts/build.ps1` (it will fall back automatically).

### MSVC preset fallback (no Ninja)
This repo also includes Visual Studio generator presets (used automatically by `scripts/build.ps1` when Ninja isn't present):

```powershell
cmake --preset windows-vcpkg-static-msvc
cmake --build --preset windows-vcpkg-static-msvc
./build/windows-vcpkg-static-msvc/Debug/LangJamSDL.exe
```

## VS Code
- Tasks: `Build (vcpkg preset)` and `Run`
- Debug: `Debug LangJamSDL` (launches with preLaunchTask to build)

## Notes
- SDL2 is declared in `vcpkg.json`; vcpkg will acquire it automatically in manifest mode during configure.
- Static linking uses vcpkg triplet `x64-windows-static` and configures the MSVC runtime to static.
- With dynamic linking, ensure `SDL2.dll` is discoverable (vcpkg typically places runtime DLLs next to the build or in your PATH).

## VM

### Targets
- `LangJamVMCore` (static library): the VM implementation (link this into other targets).
- `LangJamVM` (console app): VM demo.
- `LangJamSDL` (SDL app): renders a texture sourced from VM memory.

### Memory-mapped framebuffer
The VM exposes a fixed 32-bit framebuffer region in its unified cell memory. Each cell is one pixel in ARGB8888 format.

- Dimensions: `VM::FB_WIDTH` x `VM::FB_HEIGHT` (currently 256x192)
- Region (cell addresses): `VM::FB_BASE .. VM::FB_BASE + VM::FB_SIZE`
- Pixel address: `VM::FB_BASE + y * VM::FB_WIDTH + x`

The SDL app uploads this region into an `SDL_PIXELFORMAT_ARGB8888` streaming texture each frame.

### Saving/loading bytecode
Bytecode programs are `std::vector<vm32::u32>` (cells) and can be saved/loaded using the helpers in `vm/bytecode_io.h`:

```cpp
#include "bytecode_io.h"

std::string err;
vm32::saveBytecodeToFile(cells, "program.ljbc", &err);
vm32::loadBytecodeFromFile("program.ljbc", cells, &err);
```

The on-disk format is a small binary container with a magic header and little-endian u32 cells.

### Running
Run the SDL app (renders the VM framebuffer):

```powershell
./scripts/build.ps1 -Run
```

Run the VM demo (factorial prints 120):

```powershell
./scripts/build.ps1 -RunVm
```

## Project Structure
- `CMakeLists.txt` – CMake project configuration
- `CMakePresets.json` – presets that reference `$env:VCPKG_ROOT`
- `vcpkg.json` – vcpkg manifest with `sdl2` dependency
- `src/main.cpp` – SDL window + VM framebuffer presentation
- `.vscode/` – tasks and debug configuration
- `.gitignore` – ignores common build artifacts
- `vm/` – VM core + demo + bytecode builder
- `scripts/` – setup/build helpers
- `ROADMAP.md` – follow-up ideas
