cmake_minimum_required(VERSION 3.15)
project(lang)
include(FetchContent)

# Example: FetchContent for fmt library
FetchContent_Declare(
	fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG 10.2.1 # Use a stable release tag
)
FetchContent_MakeAvailable(fmt)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files

# Only main.cpp is a source file, headers are included automatically
add_executable(lang main.cpp)

target_include_directories(lang PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
