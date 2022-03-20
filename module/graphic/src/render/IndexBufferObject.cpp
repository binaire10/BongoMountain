#include "IndexBufferObject.hpp"
#include "private/IndexBufferObjectPrivate.hpp"

render::IndexBufferObject::IndexBufferObject(render::RenderDevice                               *owner,
                                             std::unique_ptr<detail::IndexBufferObjectPrivate> &&handle)
    : RenderElement(owner)
    , m_handler{ std::move(handle) }
{}

render::IndexBufferObject::~IndexBufferObject() = default;

void render::IndexBufferObject::bind() const { m_handler->bind(); }

void render::IndexBufferObject::create(void *data, std::size_t size) const { m_handler->create(data, size); }

void render::IndexBufferObject::create(std::size_t size) const { m_handler->create(size); }
