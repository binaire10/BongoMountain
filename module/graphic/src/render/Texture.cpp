#include "Texture.hpp"
#include "private/TexturePrivate.hpp"

render::Texture::Texture(render::RenderDevice *owner, std::unique_ptr<detail::TexturePrivate> &&handler)
    : RenderElement{ owner }
    , m_handler{ std::move(handler) }
{
    BM_CORE_ASSERT(m_handler, "Texture handler is require");
}

render::Texture::~Texture() = default;

void render::Texture::create(const graphic::ImageView &imageView) { m_handler->create(imageView); }

void render::Texture::bind() const { m_handler->bind(); }
void render::Texture::bind(unsigned int unit) const { m_handler->bind(unit); }
