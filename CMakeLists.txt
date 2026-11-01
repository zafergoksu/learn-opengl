cmake_minimum_required(VERSION 3.26)

project(learn-opengl
    VERSION 0.1.0
    DESCRIPTION "learn-opengl"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Warning flags shared by all first-party targets (deps stay quiet).
add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE
    -Wall -Wextra -Wpedantic -Wshadow -Wconversion)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(src)
add_subdirectory(app)
