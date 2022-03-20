#ifndef BONGOMOUNTAIN_INDEXBUFFEROBJECT_HPP
#define BONGOMOUNTAIN_INDEXBUFFEROBJECT_HPP

#include <memory>
#include "../graphic/config.hpp"
#include <core/Assert.hpp>
#include "RenderElement.hpp"

namespace render::detail
{
    class IndexBufferObjectPrivate;
}

namespace render
{
    class BM_GRAPHIC_DCL IndexBufferObject final : public RenderElement
    {
    public:
        IndexBufferObject(RenderDevice *owner, std::unique_ptr<detail::IndexBufferObjectPrivate> &&handle);
        ~IndexBufferObject() override;
        void bind() const;
        void create(void *data, std::size_t size) const;
        void create(std::size_t size) const;

    private:
        std::unique_ptr<detail::IndexBufferObjectPrivate> m_handler;
    };
}// namespace render

#endif// BONGOMOUNTAIN_INDEXBUFFEROBJECT_HPP
