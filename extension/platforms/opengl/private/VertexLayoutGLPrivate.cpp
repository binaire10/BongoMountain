#include "VertexLayoutGLPrivate.hpp"

void VertexLayoutGLPrivate::create(const render::AttributeLayout *attrs, std::size_t count, std::size_t stride)
{
    m_handle = opengl::BasicVertexLayout::create(attrs, count, stride);
}

void VertexLayoutGLPrivate::bind() const { m_handle.bind(); }

void VertexLayoutGLPrivate::destroy() { m_handle.destroy(); }
