add_library(learn_opengl
    learn_opengl.cpp)
add_library(learn_opengl::learn_opengl ALIAS learn_opengl)

target_include_directories(learn_opengl
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)

target_link_libraries(learn_opengl PRIVATE project_warnings)
