#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace learn_opengl {

struct VertexAttribute {
    unsigned int location;
    int components;
    std::size_t offset;
};

class Mesh {
public:
    Mesh(std::span<const float> vertices, std::span<const std::uint32_t> indices,
         std::span<const VertexAttribute> layout, std::size_t strideBytes);

    ~Mesh();
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) noexcept;
    Mesh& operator=(Mesh&&) noexcept;

    void draw() const;

private:
    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;
    unsigned int ebo_ = 0;
    int vertexCount_ = 0;
    int indexCount_ = 0;
};

}  // namespace learn_opengl
