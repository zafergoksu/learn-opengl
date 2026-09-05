find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(simple
    main.cpp)

target_link_libraries(simple PRIVATE glfw)
target_link_libraries(simple PRIVATE glad::glad)

add_custom_command(TARGET simple POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:simple>/shaders)
