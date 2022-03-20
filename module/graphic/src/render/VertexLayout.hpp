#ifndef BONGOMOUNTAIN_VERTEXLAYOUT_HPP
#define BONGOMOUNTAIN_VERTEXLAYOUT_HPP

#include <memory>
#include "../graphic/config.hpp"
#include <core/Assert.hpp>
#include "RenderElement.hpp"
#include "VertexLayoutDescriptor.h"

namespace render::detail
{
    class VertexLayoutPrivate;
}

namespace render
{
    class BM_GRAPHIC_DCL VertexLayout final : public RenderElement
    {
    public:
        VertexLayout(RenderDevice *owner, std::unique_ptr<detail::VertexLayoutPrivate> &&handler);
        ~VertexLayout() override;

        VertexLayout(VertexLayout &&) noexcept = default;
        VertexLayout &operator=(VertexLayout &&) noexcept = default;

        void create(const AttributeLayout *attrs, std::size_t size, std::size_t stride);
        void bind() const;
        void destroy();

        [[nodiscard]] constexpr const std::unique_ptr<detail::VertexLayoutPrivate> &getPrivate() const
        {
            return m_handler;
        }

    protected:
        friend class render::RenderDevice;
        std::unique_ptr<detail::VertexLayoutPrivate> m_handler;
    };
}// namespace render

#endif// BONGOMOUNTAIN_VERTEXLAYOUT_HPP
