find_package(GTest REQUIRED)

add_executable(learn_opengl_tests
    test_greeting.cpp)

target_link_libraries(learn_opengl_tests
    PRIVATE
        learn_opengl::learn_opengl
        GTest::gtest_main
        project_warnings)

include(GoogleTest)
gtest_discover_tests(learn_opengl_tests)
