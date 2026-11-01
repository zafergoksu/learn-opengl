#pragma once

#include <expected>
#include <filesystem>

namespace learn_opengl {

class ShaderProgram {
public:
    static std::expected<ShaderProgram, std::string> fromFiles(
        const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

    ~ShaderProgram();
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;
    ShaderProgram(ShaderProgram&& other) noexcept;
    ShaderProgram& operator=(ShaderProgram&& other) noexcept;

    void use() const;

private:
    explicit ShaderProgram(unsigned int id);

    unsigned int id_;
};

}  // namespace learn_opengl
