add_executable(learn-opengl
    main.cpp)

target_link_libraries(learn-opengl
    PRIVATE
        learn_opengl::learn_opengl
        project_warnings)
