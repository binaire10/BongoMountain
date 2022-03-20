#ifndef BONGOMOUNTAIN_OPENGL_VERTEXARRAY_HPP
#define BONGOMOUNTAIN_OPENGL_VERTEXARRAY_HPP

#include <render/VertexLayoutDescriptor.h>
#include "BufferObject.hpp"

namespace opengl
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

    static constexpr GLenum shaderDataTypeToOpenGLBaseType(render::ShaderDataType type)
    {
        using render::ShaderDataType;
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

    class BasicVertexLayout
    {
    public:
        using VertexArrayHolder = core::resource<GLuint, vao_trait>;

        template<typename VertexLayoutT>
        static BasicVertexLayout create(const VertexLayoutT &layout)
        {
            return create(std::data(layout), std::size(layout), layout.stride);
        }

        static inline BasicVertexLayout
            create(const render::AttributeLayout *attrs, std::size_t count, std::size_t stride)
        {

            BasicVertexLayout bvao;
            auto             &vao = bvao.vao;
            vao.create();

            bvao.bind();
            std::for_each(attrs, attrs + count, [i = 0u, stride](const render::AttributeLayout &e) mutable {
                glEnableVertexAttribArray(i);
                glVertexAttribPointer(i++,
                                      shaderComponentCount(e.type),
                                      shaderDataTypeToOpenGLBaseType(e.type),
                                      e.normalized ? GL_TRUE : GL_FALSE,
                                      stride,
                                      reinterpret_cast<void *>(e.offset));
            });
            return bvao;
        }

        void bind() const noexcept { glBindVertexArray(vao.getResource()); }

        void destroy() { vao.destroy(); }

    protected:
        VertexArrayHolder  vao;
    };

    template<typename I>
    class VertexLayout : public BasicVertexLayout
    {
    public:
        VertexLayout() = default;
        VertexLayout(BasicVertexLayout &&vao) noexcept
            : BasicVertexLayout(std::move(vao))
        {}

        template<typename VertexLayoutT, std::size_t n>
        static VertexLayout create(const VertexLayoutT &layout, SharedVectorIndexBuffer<I> ibo, SharedVertexBuffer vbo)
        {
            VertexLayout vao = BasicVertexLayout::create(layout);

            vao.count    = ibo.size();
            vao.indices  = std::move(ibo);
            vao.vertices = std::move(vbo);
            return vao;
        }

    private:
        std::size_t        count;
        SharedIndexBuffer  indices;
        SharedVertexBuffer vertices;
    };
}// namespace opengl

#endif// BONGOMOUNTAIN_OPENGL_VERTEXARRAY_HPP
