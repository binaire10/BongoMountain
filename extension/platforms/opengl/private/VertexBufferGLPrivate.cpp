#include "VertexBufferGLPrivate.hpp"

using opengl::VectorVertexBuffer;

void VertexBufferGLPrivate::create(std::size_t size) { vertexBuffer = VectorVertexBuffer<uint8_t>::create(size); }

void VertexBufferGLPrivate::create(const void *data, std::size_t size)
{
    vertexBuffer = VectorVertexBuffer<uint8_t>::create(reinterpret_cast<const uint8_t *>(data), size);
}

void VertexBufferGLPrivate::overwrite(std::size_t offset, const void *data, std::size_t size)
{
    vertexBuffer.overwrite(offset, reinterpret_cast<const uint8_t *>(data), size);
}

void VertexBufferGLPrivate::insert(std::size_t offset, const void *data, std::size_t size)
{
    vertexBuffer.insert(offset, reinterpret_cast<const uint8_t *>(data), size);
}

void VertexBufferGLPrivate::erase(std::size_t offset, std::size_t len) { vertexBuffer.erase(offset, len); }

void VertexBufferGLPrivate::destroy() { vertexBuffer.destroy(); }

void *VertexBufferGLPrivate::map() { return vertexBuffer.map(); }

const void *VertexBufferGLPrivate::map() const { return vertexBuffer.map(); }

void VertexBufferGLPrivate::unmap(const void *pVoid) const { return vertexBuffer.unmap(pVoid); }

void VertexBufferGLPrivate::bind() const { vertexBuffer.bind(); }

std::size_t VertexBufferGLPrivate::size() const { return vertexBuffer.size(); }
