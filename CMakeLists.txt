cmake_minimum_required(VERSION 3.21)

project(LangJamSDL VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
  # Use static MSVC runtime to fully static-link when using vcpkg's static triplet
  # MultiThreaded for Release, MultiThreadedDebug for Debug
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# If VCPKG_ROOT is set, use its toolchain automatically
if(DEFINED ENV{VCPKG_ROOT} AND EXISTS "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
  message(STATUS "Using vcpkg from $ENV{VCPKG_ROOT}")
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "" FORCE)
endif()

# Find SDL2 provided by vcpkg or a system install with CMake config files
find_package(SDL2 CONFIG REQUIRED)

add_executable(LangJamSDL
  src/main.cpp
)

# Link against SDL2 targets from the CONFIG package
# SDL2::SDL2main provides WinMain handling on Windows
# SDL2::SDL2 is the main library
if(TARGET SDL2::SDL2main)
  target_link_libraries(LangJamSDL PRIVATE SDL2::SDL2main)
endif()

# Prefer static SDL2 if available (with vcpkg x64-windows-static triplet)
if(TARGET SDL2::SDL2-static)
  target_link_libraries(LangJamSDL PRIVATE SDL2::SDL2-static)
elseif(TARGET SDL2::SDL2)
  target_link_libraries(LangJamSDL PRIVATE SDL2::SDL2)
else()
  message(FATAL_ERROR "SDL2 target not found. Ensure SDL2 is installed and discoverable by CMake.")
endif()

# Warnings
if(MSVC)
  target_compile_options(LangJamSDL PRIVATE /W4 /permissive-)
else()
  target_compile_options(LangJamSDL PRIVATE -Wall -Wextra -Wpedantic)
endif()

# VM core library (static) to be reused by demos and SDL app
add_library(LangJamVMCore STATIC
  vm/vm.cpp
  vm/bytecode_io.cpp
)
target_include_directories(LangJamVMCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vm)
if(MSVC)
  target_compile_options(LangJamVMCore PRIVATE /W4 /permissive-)
else()
  target_compile_options(LangJamVMCore PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Link VM core into SDL app so it can embed the VM
target_link_libraries(LangJamSDL PRIVATE LangJamVMCore)

# VM demo target (console app) using the VM core
add_executable(LangJamVM
  vm/demo_main.cpp
)
target_link_libraries(LangJamVM PRIVATE LangJamVMCore)
if(MSVC)
  target_compile_options(LangJamVM PRIVATE /W4 /permissive-)
else()
  target_compile_options(LangJamVM PRIVATE -Wall -Wextra -Wpedantic)
endif()
