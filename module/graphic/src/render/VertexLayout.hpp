#ifndef BONGOMOUNTAIN_VERTEXLAYOUT_HPP
#define BONGOMOUNTAIN_VERTEXLAYOUT_HPP

#include <memory>
#include "../graphic/config.hpp"
#include <core/Assert.hpp>
#include "RenderElement.hpp"
#include "VertexLayoutDescriptor.h"
#include "private/VertexLayoutPrivate.hpp"

namespace render
{
    class BM_GRAPHIC_DCL VertexLayout final : public RenderElement
    {
    public:
        VertexLayout() = default;
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

    private:
        std::unique_ptr<detail::VertexLayoutPrivate> m_handler;
    };
}// namespace render

#endif// BONGOMOUNTAIN_VERTEXLAYOUT_HPP
