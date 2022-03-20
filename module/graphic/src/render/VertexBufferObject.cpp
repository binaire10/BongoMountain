#include "VertexBufferObject.hpp"
#include "private/VertexBufferPrivate.hpp"

render::VertexBufferObject::VertexBufferObject(render::RenderDevice                          *owner,
                                               std::unique_ptr<detail::VertexBufferPrivate> &&handler)
    : RenderElement(owner)
    , m_handler{ std::move(handler) }
{
    BM_CORE_ASSERT(m_handler, "VertexBufferObject handler is require");
}

render::VertexBufferObject::~VertexBufferObject() = default;

void render::VertexBufferObject::bind() const { m_handler->bind(); }

void render::VertexBufferObject::create(std::size_t size) { m_handler->create(size); }

void render::VertexBufferObject::create(const void *data, std::size_t size) { m_handler->create(data, size); }

void render::VertexBufferObject::overwrite(std::size_t offset, const void *data, std::size_t size)
{
    m_handler->overwrite(offset, data, size);
}
void render::VertexBufferObject::destroy() { m_handler->destroy(); }
