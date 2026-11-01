#include <glad/glad.h>

#include <spdlog/async.h>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <array>
#include <cstdint>
#include <cstdlib>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Window.hpp"

using learn_opengl::Mesh;
using learn_opengl::ShaderProgram;
using learn_opengl::VertexAttribute;
using learn_opengl::Window;

#ifdef NDEBUG
constexpr std::size_t asyncLoggerMessageQueueSize = 8192;
constexpr std::size_t asyncLoggerThreadPoolSize = 1;
#endif

struct LoggingGuard {
    ~LoggingGuard() { spdlog::shutdown(); }
};

int main() {
    LoggingGuard loggingGuard;
    constexpr std::array<float, 18> vertices{
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f   // top
    };
    constexpr std::array<std::uint32_t, 3> indices{0, 1, 2};
    constexpr std::array<VertexAttribute, 2> layout{VertexAttribute{0, 3, 0},
                                                    VertexAttribute{1, 3, 3 * sizeof(float)}};

#ifdef NDEBUG
    spdlog::init_thread_pool(asyncLoggerMessageQueueSize, asyncLoggerThreadPoolSize);
    auto logger = spdlog::create_async_nb<spdlog::sinks::stderr_color_sink_mt>("console");
#else
    auto logger = spdlog::stderr_color_mt("console");
#endif
    // set spdlog to use stdout
    spdlog::set_default_logger(logger);
    spdlog::set_pattern("[%H:%M:%S.%e] [%^%l%$] [%s:%#] %v");
    spdlog::cfg::load_env_levels();

#ifdef NDEBUG
    SPDLOG_INFO("Setting logger to async with MQSZ: {}, TPSZ: {}", asyncLoggerMessageQueueSize,
                asyncLoggerThreadPoolSize);
#else
    SPDLOG_INFO("Setting logger to sync");
#endif

    auto window = Window::create(800, 600, "LearnOpenGL");
    if (!window) {
        SPDLOG_CRITICAL("{}", window.error());
        return EXIT_FAILURE;
    }

    auto shader = ShaderProgram::fromFiles("shaders/triangle.vert", "shaders/triangle.frag");
    if (!shader) {
        SPDLOG_CRITICAL("{}", shader.error());
        return EXIT_FAILURE;
    }

    const Mesh triangle{vertices, indices, layout, 6 * sizeof(float)};

    while (window->isOpen()) {
        window->processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->use();
        triangle.draw();

        window->swapAndPoll();
    }

    return EXIT_SUCCESS;
}
