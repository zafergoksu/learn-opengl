#include "Mesh.hpp"

#include <glad/glad.h>

#include <spdlog/spdlog.h>

#include <cstddef>
#include <cstdint>
#include <utility>

namespace learn_opengl {

Mesh::Mesh(std::span<const float> vertices, std::span<const std::uint32_t> indices,
           std::span<const VertexAttribute> layout, std::size_t strideBytes)
    : vertexCount_{static_cast<int>(vertices.size_bytes() / strideBytes)},
      indexCount_{static_cast<int>(indices.size())} {
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size_bytes()), vertices.data(),
                 GL_STATIC_DRAW);

    if (!indices.empty()) {
        // The bound VAO records this EBO binding, so it stays bound until the VAO unbinds.
        glGenBuffers(1, &ebo_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size_bytes()),
                     indices.data(), GL_STATIC_DRAW);
    }

    for (const VertexAttribute& attribute : layout) {
        // Where the vertex attribute is (the layout of the VBO it would point to) and how
        // many components (variables) associated with it (how many elements are part of the VBO stride).
        glVertexAttribPointer(attribute.location, attribute.components, GL_FLOAT, GL_FALSE,
                              static_cast<GLsizei>(strideBytes),
                              reinterpret_cast<const void*>(attribute.offset));
        glEnableVertexAttribArray(attribute.location);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    SPDLOG_DEBUG("Created mesh: {} vertices, {} indices", vertexCount_, indexCount_);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
}

Mesh::Mesh(Mesh&& other) noexcept
    : vao_{std::exchange(other.vao_, 0)},
      vbo_{std::exchange(other.vbo_, 0)},
      ebo_{std::exchange(other.ebo_, 0)},
      vertexCount_{std::exchange(other.vertexCount_, 0)},
      indexCount_{std::exchange(other.indexCount_, 0)} {}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    std::swap(vao_, other.vao_);
    std::swap(vbo_, other.vbo_);
    std::swap(ebo_, other.ebo_);
    std::swap(vertexCount_, other.vertexCount_);
    std::swap(indexCount_, other.indexCount_);
    return *this;
}

void Mesh::draw() const {
    glBindVertexArray(vao_);
    if (indexCount_ > 0) {
        glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, nullptr);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, vertexCount_);
    }
}

}  // namespace learn_opengl
