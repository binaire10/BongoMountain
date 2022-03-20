#include "VertexLayout.hpp"
#include "private/VertexLayoutPrivate.hpp"

render::VertexLayout::VertexLayout(render::RenderDevice *owner, std::unique_ptr<detail::VertexLayoutPrivate> &&handler)
    : RenderElement(owner)
    , m_handler(std::move(handler))
{
    BM_CORE_ASSERT(m_handler, "VertexLayout require an handle");
}

render::VertexLayout::~VertexLayout() = default;

void render::VertexLayout::create(const AttributeLayout *attrs, std::size_t size, std::size_t stride)
{
    m_handler->create(attrs, size, stride);
}

void render::VertexLayout::bind() const { m_handler->bind(); }

void render::VertexLayout::destroy() { m_handler->destroy(); }
