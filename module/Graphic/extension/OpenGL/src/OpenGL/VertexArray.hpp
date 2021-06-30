#ifndef GAMEENGINEBONGO_VERTEXARRAY_HPP
#define GAMEENGINEBONGO_VERTEXARRAY_HPP

#include "config.hpp"

#include "Shader.hpp"
#include "BufferObject.hpp"

class VertexArray
{
    struct vao_trait
    {
        static constexpr GLuint invalid_resource() noexcept { return 0; }

        static inline GLuint create() noexcept
        {
            GLuint id;
            glGenVertexArrays(1, &id);
            return id;
        }

        static inline void destroy(const GLuint &id) noexcept { glDeleteVertexArrays(1, &id); }
    };


    static constexpr GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch(type)
        {
        case ShaderDataType::Float:
            return GL_FLOAT;
        case ShaderDataType::Float2:
            return GL_FLOAT;
        case ShaderDataType::Float3:
            return GL_FLOAT;
        case ShaderDataType::Float4:
            return GL_FLOAT;
        case ShaderDataType::Mat3:
            return GL_FLOAT;
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
            return GL_INT;
        case ShaderDataType::Int2:
            return GL_INT;
        case ShaderDataType::Int3:
            return GL_INT;
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
        }
        throw std::range_error{ "Unknown ShaderDataType!" };
    }

public:
    using VertexArrayHolder = Resource<GLuint, vao_trait>;

    template<std::size_t n, typename T>
    void create(const BufferLayout<n> &layout, IndexBufferStorageObject<T> ibo, VertexBufferObject vbo)
    {
        indices  = std::move(ibo);
        vertices = std::move(vbo);
        count    = ibo.getCount();

        vao.create();

        bind();
        unsigned i{};
        for(const BufferElement &e : layout)
        {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i++,
                                  shaderComponentCount(e.type),
                                  shaderDataTypeToOpenGLBaseType(e.type),
                                  e.normalized ? GL_TRUE : GL_FALSE,
                                  layout.stride,
                                  reinterpret_cast<void *>(e.offset));
        }
    }

    void bind() const noexcept { glBindVertexArray(vao.getResource()); }

    [[nodiscard]] const IndexBufferObject &getIndices() const noexcept { return indices; }

    [[nodiscard]] const VertexBufferObject &getVertices() const noexcept { return vertices; }

    [[nodiscard]] unsigned getCount() const noexcept { return count; }

private:
    VertexArrayHolder  vao;
    IndexBufferObject  indices;
    VertexBufferObject vertices;
    unsigned           count;
};

#endif// GAMEENGINEBONGO_VERTEXARRAY_HPP
