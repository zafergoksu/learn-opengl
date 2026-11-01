#pragma once

#include <expected>
#include <string>

struct GLFWwindow;

namespace learn_opengl {

class Window {
public:
    static std::expected<Window, std::string> create(int width, int height,
                                                     const std::string& title);

    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

    [[nodiscard]] bool isOpen() const;
    void processInput();
    void swapAndPoll();

private:
    explicit Window(GLFWwindow* window);

    GLFWwindow* windowHandle_ = nullptr;
};

}  // namespace learn_opengl
