#include "IndexBufferObjectGLPrivate.hpp"

void IndexBufferObjectGLPrivate::create(std::size_t size) { m_handle = opengl::IndexBuffer::create(size); }

void IndexBufferObjectGLPrivate::create(const void *data, std::size_t size)
{
    m_handle = opengl::IndexBuffer::create(data, size);
}

void IndexBufferObjectGLPrivate::bind() const { m_handle.bind(); }

void IndexBufferObjectGLPrivate::destroy() { m_handle.destroy(); }
