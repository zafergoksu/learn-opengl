add_library(learn_opengl STATIC
    Mesh.cpp
    Shader.cpp
    Window.cpp)

target_include_directories(learn_opengl PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(learn_opengl
    PUBLIC glad::glad spdlog::spdlog
    PRIVATE glfw project_warnings)

# Keep every SPDLOG_* macro in the build. The runtime level still filters output.
target_compile_definitions(learn_opengl PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
