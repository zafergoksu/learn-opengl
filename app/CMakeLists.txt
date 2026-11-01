find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(simple
    Main.cpp)

target_link_libraries(simple PRIVATE learn_opengl project_warnings)

add_custom_command(TARGET simple POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:simple>/shaders)
