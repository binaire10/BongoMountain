#ifndef BONGOMOUNTAIN_INDEXBUFFEROBJECT_HPP
#define BONGOMOUNTAIN_INDEXBUFFEROBJECT_HPP

#include <memory>
#include "../graphic/config.hpp"
#include <core/Assert.hpp>
#include "RenderElement.hpp"
#include "private/IndexBufferObjectPrivate.hpp"

namespace render
{
    class BM_GRAPHIC_DCL IndexBufferObject final : public RenderElement
    {
    public:
        IndexBufferObject() = default;
        IndexBufferObject(RenderDevice *owner, std::unique_ptr<detail::IndexBufferObjectPrivate> &&handle);
        ~IndexBufferObject() override;
        void bind() const;
        void create(void *data, std::size_t size) const;
        void create(std::size_t size) const;
        void destroy();

    private:
        std::unique_ptr<detail::IndexBufferObjectPrivate> m_handler;
    };
}// namespace render

#endif// BONGOMOUNTAIN_INDEXBUFFEROBJECT_HPP
