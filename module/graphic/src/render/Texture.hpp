#ifndef BONGOMOUNTAIN_TEXTURE_HPP
#define BONGOMOUNTAIN_TEXTURE_HPP

#include <memory>
#include "../graphic/config.hpp"
#include <core/Assert.hpp>
#include <graphic/Image.hpp>
#include "RenderElement.hpp"

namespace render::detail
{
    class TexturePrivate;
}

namespace render
{
    class BM_GRAPHIC_DCL Texture final : public RenderElement
    {
    public:
        Texture(RenderDevice *owner, std::unique_ptr<detail::TexturePrivate> &&handler);
        ~Texture() override;

        Texture(Texture &&) noexcept = default;
        Texture &operator=(Texture &&) noexcept = default;

        void bind() const;
        void bind(unsigned unit) const;
        void create(const graphic::ImageView &imageView);
        [[nodiscard]] constexpr const std::unique_ptr<detail::TexturePrivate> &getPrivate() const { return m_handler; }

    private:
        std::unique_ptr<detail::TexturePrivate> m_handler;
    };
}// namespace render

#endif// BONGOMOUNTAIN_TEXTURE_HPP
