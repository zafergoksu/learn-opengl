#include "Window.hpp"

#include <glad/glad.h>

#include <expected>
#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <utility>

namespace learn_opengl {
namespace {

void glfwErrorCallback(int code, const char* description) {
    SPDLOG_ERROR("GLFW ({}): {}", code, description);
}

void framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}

#ifndef NDEBUG
void APIENTRY debugCallback(GLenum /*source*/, GLenum /*type*/, GLuint /*id*/, GLenum severity,
                            GLsizei /*length*/, const GLchar* message, const void* /*user*/) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            SPDLOG_ERROR("GL: {}", message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            SPDLOG_WARN("GL: {}", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            SPDLOG_INFO("GL: {}", message);
            break;
        default:
            SPDLOG_DEBUG("GL: {}", message);
            break;
    }
}
#endif
}  // namespace

Window::Window(GLFWwindow* window) : windowHandle_{window} {}

std::expected<Window, std::string> Window::create(int width, int height, const std::string& title) {
    glfwSetErrorCallback(glfwErrorCallback);
    if (glfwInit() != GLFW_TRUE) {
        return std::unexpected{"Failed to initialize GLFW"};
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    GLFWwindow* windowHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (windowHandle == nullptr) {
        glfwTerminate();
        return std::unexpected{"Failed to create GLFW window"};
    }

    Window window{windowHandle};
    glfwMakeContextCurrent(windowHandle);

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
        return std::unexpected{"Failed to initialize GLAD"};
    }

#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, nullptr);
#endif

    SPDLOG_INFO("Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

    int frameBufferWidth = 0;
    int frameBufferHeight = 0;
    glfwGetFramebufferSize(windowHandle, &frameBufferWidth, &frameBufferHeight);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);
    glfwSetFramebufferSizeCallback(windowHandle, framebufferSizeCallback);

    return window;
}

Window::~Window() {
    if (windowHandle_ != nullptr) {
        glfwDestroyWindow(windowHandle_);
        glfwTerminate();
    }
}

Window::Window(Window&& other) noexcept
    : windowHandle_{std::exchange(other.windowHandle_, nullptr)} {}

Window& Window::operator=(Window&& other) noexcept {
    std::swap(windowHandle_, other.windowHandle_);
    return *this;
}

bool Window::isOpen() const {
    return glfwWindowShouldClose(windowHandle_) == GLFW_FALSE;
}

void Window::processInput() {
    if (glfwGetKey(windowHandle_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(windowHandle_, GLFW_TRUE);
    }
}

void Window::swapAndPoll() {
    glfwSwapBuffers(windowHandle_);
    glfwPollEvents();
}

}  // namespace learn_opengl
