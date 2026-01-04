cmake_minimum_required(VERSION 3.15)
project(runtime)

include(FetchContent)

# FetchContent for SDL2
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.30.2 # Use a stable release tag
)
FetchContent_MakeAvailable(SDL2)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Add source files

# Only .cpp files are sources, headers are included automatically
set(SOURCES
        vm.cpp
        main.cpp
)

add_executable(runtime ${SOURCES})
# target_include_directories(runtime PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(runtime PRIVATE ../bytecode/)
target_link_libraries(runtime PRIVATE SDL2::SDL2 SDL2::SDL2main)