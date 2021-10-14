#ifndef GAMEENGINEBONGO_VERTEXARRAY_HPP
#define GAMEENGINEBONGO_VERTEXARRAY_HPP

#include "config.hpp"

#include <Graphic/BufferLayout.h>
#include "BufferObject.hpp"
#include <Graphic/BufferArray.hpp>

namespace OpenGL
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

    class BasicVertexArray
    {
    public:
        using VertexArrayHolder = Core::resource<GLuint, vao_trait>;

        template<std::size_t n, typename T>
        static BasicVertexArray create(const BufferLayout<n> &layout, Graphic::SharedVertexBufferArray<T> vbo)
        {
            return create(layout, vbo, vbo.getCount());
        }

        template<std::size_t n>
        static BasicVertexArray create(const BufferLayout<n> &layout, SharedVertexBuffer vbo, unsigned count)
        {
            BasicVertexArray bvao;
            bvao.count    = count;
            bvao.vertices = vbo;
            auto &vao     = bvao.vao;
            vao.create();

            bvao.bind();
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
            return bvao;
        }

        void bind() const noexcept { glBindVertexArray(vao.getResource()); }

        [[nodiscard]] const SharedVertexBuffer &getVertices() const noexcept { return vertices; }

        [[nodiscard]] unsigned getCount() const noexcept { return count; }

    protected:
        VertexArrayHolder  vao;
        std::size_t        count;
        SharedVertexBuffer vertices;
    };

    template<typename I>
    class VertexArray : public BasicVertexArray
    {
    public:
        VertexArray() = default;
        VertexArray(BasicVertexArray &&vao) noexcept
            : BasicVertexArray(std::move(vao))
        {}

        template<std::size_t n>
        static VertexArray
            create(const BufferLayout<n> &layout, Graphic::SharedIndexBufferArray<I> ibo, SharedVertexBuffer vbo)
        {
            VertexArray vao = BasicVertexArray::create(layout, std::move(vbo), ibo.getCount());
            vao.indices = std::move(ibo);
            return vao;
        }

        [[nodiscard]] const SharedIndexBuffer &getIndices() const noexcept { return indices; }

    private:
        SharedIndexBuffer indices;
    };
}// namespace OpenGL

#endif// GAMEENGINEBONGO_VERTEXARRAY_HPP
