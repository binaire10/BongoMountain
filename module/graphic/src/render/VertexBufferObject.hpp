#ifndef BONGOMOUNTAIN_VERTEXBUFFEROBJECT_HPP
#define BONGOMOUNTAIN_VERTEXBUFFEROBJECT_HPP

#include <memory>
#include "../graphic/config.hpp"
#include <core/Assert.hpp>
#include "RenderElement.hpp"

namespace render::detail
{
    class VertexBufferPrivate;
}

namespace render
{
    class BM_GRAPHIC_DCL VertexBufferObject final : public RenderElement
    {
    public:
        VertexBufferObject(RenderDevice *owner, std::unique_ptr<detail::VertexBufferPrivate> &&handler);
        ~VertexBufferObject() override;

        VertexBufferObject(VertexBufferObject &&) noexcept = default;
        VertexBufferObject &operator=(VertexBufferObject &&) noexcept = default;

        void bind() const;
        void create(std::size_t size);
        void create(const void *data, std::size_t size);
        void overwrite(std::size_t offset, const void *data, std::size_t size);
        void destroy();
        [[nodiscard]] constexpr const std::unique_ptr<detail::VertexBufferPrivate> &getPrivate() const
        {
            return m_handler;
        }

    protected:
        friend class render::RenderDevice;
        std::unique_ptr<detail::VertexBufferPrivate> m_handler;
    };
}// namespace render

#endif// BONGOMOUNTAIN_VERTEXBUFFEROBJECT_HPP
