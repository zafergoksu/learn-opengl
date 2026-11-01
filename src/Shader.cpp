#include "Shader.hpp"

#include <glad/glad.h>

#include <GL/gl.h>
#include <spdlog/spdlog.h>

#include <cstddef>
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

namespace learn_opengl {
namespace {

class ShaderStage {
public:
    explicit ShaderStage(GLenum type) : id_{glCreateShader(type)} {}

    ~ShaderStage() { glDeleteShader(id_); }

    ShaderStage(const ShaderStage&) = delete;
    ShaderStage& operator=(const ShaderStage&) = delete;

    [[nodiscard]] GLuint id() const { return id_; };

private:
    GLuint id_;
};

std::expected<std::string, std::string> readFile(const std::filesystem::path& path) {
    std::ifstream file{path};

    if (!file.is_open()) {
        return std::unexpected{std::format("Failed to open shader file: {}", path.string())};
    }

    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

std::string shaderLog(GLuint shader) {
    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    std::string log(static_cast<std::size_t>(length), '\0');
    GLsizei written = 0;

    // Get the shader log from OpenGL and record how much was written.
    glGetShaderInfoLog(shader, length, &written, log.data());
    log.resize(static_cast<std::size_t>(written));
    return log;
}

std::string programLog(GLuint program) {
    GLint length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    std::string log(static_cast<std::size_t>(length), '\0');
    GLsizei written = 0;  // Excludes the null terminator counted by the length query.
    glGetProgramInfoLog(program, length, &written, log.data());
    log.resize(static_cast<std::size_t>(written));
    return log;
}

std::expected<void, std::string> compile(const ShaderStage& stage,
                                         const std::filesystem::path& path) {
    auto source = readFile(path);
    if (!source) {
        return std::unexpected{std::move(source.error())};
    }

    const char* text = source->c_str();
    glShaderSource(stage.id(), 1, &text, nullptr);
    glCompileShader(stage.id());

    GLint success = GL_FALSE;
    glGetShaderiv(stage.id(), GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        return std::unexpected{
            std::format("Failed to compile {}:\n{}", path.string(), shaderLog(stage.id()))};
    }

    if (auto log = shaderLog(stage.id()); !log.empty()) {
        SPDLOG_WARN("{}:\n{}", path.string(), log);
    }
    return {};
}

}  // namespace

//
ShaderProgram::ShaderProgram(unsigned int id) : id_{id} {}

std::expected<ShaderProgram, std::string> ShaderProgram::fromFiles(
    const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {
    const ShaderStage vertex{GL_VERTEX_SHADER};
    if (auto result = compile(vertex, vertexPath); !result) {
        return std::unexpected{std::move(result.error())};
    }

    const ShaderStage fragment{GL_FRAGMENT_SHADER};
    if (auto result = compile(fragment, fragmentPath); !result) {
        return std::unexpected{std::move(result.error())};
    }

    ShaderProgram program{glCreateProgram()};
    glAttachShader(program.id_, vertex.id());
    glAttachShader(program.id_, fragment.id());
    glLinkProgram(program.id_);

    GLint success = GL_FALSE;
    glGetProgramiv(program.id_, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        return std::unexpected{
            std::format("Failed to link shader program:\n{}", programLog(program.id_))};
    }

    if (auto log = programLog(program.id_); !log.empty()) {
        SPDLOG_WARN("{} + {}\n{}", vertexPath.string(), fragmentPath.string(), log);
    }

    SPDLOG_DEBUG("Linked {} + {}", vertexPath.string(), fragmentPath.string());
    return program;
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id_);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept : id_{std::exchange(other.id_, 0)} {}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
    std::swap(id_, other.id_);
    return *this;
}

void ShaderProgram::use() const {
    glUseProgram(id_);
}

}  // namespace learn_opengl
