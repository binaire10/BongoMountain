#include "VertexBufferGLPrivate.hpp"

using opengl::VertexBuffer;

void VertexBufferGLPrivate::create(std::size_t size) { vertexBuffer = VertexBuffer::create(size); }

void VertexBufferGLPrivate::create(const void *data, std::size_t size)
{
    vertexBuffer = VertexBuffer::create(data, size);
}

void VertexBufferGLPrivate::overwrite(std::size_t offset, const void *data, std::size_t size)
{
    vertexBuffer.overwrite(offset, data, size);
}

void VertexBufferGLPrivate::destroy() { vertexBuffer.destroy(); }

void *VertexBufferGLPrivate::map() { return vertexBuffer.map(); }

const void *VertexBufferGLPrivate::map() const { return vertexBuffer.map(); }

void VertexBufferGLPrivate::unmap(const void *pVoid) const { return vertexBuffer.unmap(pVoid); }

void VertexBufferGLPrivate::bind() const { vertexBuffer.bind(); }
