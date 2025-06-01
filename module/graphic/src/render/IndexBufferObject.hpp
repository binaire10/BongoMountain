#ifndef BONGOMOUNTAIN_INDEXBUFFEROBJECT_HPP
#define BONGOMOUNTAIN_INDEXBUFFEROBJECT_HPP

#include <memory>
#include "../graphic/config.hpp"
#include <core/Assert.hpp>
#include "RenderElement.hpp"
#include "private/IndexBufferObjectPrivate.hpp"

namespace render
{
    class BM_GRAPHIC_DCL IndexBufferObject : public RenderElement
    {
    public:
        IndexBufferObject() = default;
        IndexBufferObject(IndexBufferObject &&) noexcept            = default;
        IndexBufferObject &operator=(IndexBufferObject &&) noexcept = default;
        IndexBufferObject(RenderDevice *owner, std::unique_ptr<detail::IndexBufferObjectPrivate> &&handle);
        ~IndexBufferObject() override;
        void bind() const;
        void destroy();

    protected:
        void create_primitive(const void *data, std::size_t size) const;
        void create_primitive(std::size_t size) const;

    private:
        std::unique_ptr<detail::IndexBufferObjectPrivate> m_handler;
        uint8_t                                           size;
    };

    template<typename T>
    class IndexBufferObjectTyped final : public IndexBufferObject
    {
    public:
        IndexBufferObjectTyped() = default;
        explicit IndexBufferObjectTyped(IndexBufferObject &&move) noexcept
            : IndexBufferObject(std::move(move))
        {}
        void create(const T *data, std::size_t size) const
        {
            IndexBufferObject::create_primitive(data, size * sizeof(T));
        }

        void create(std::size_t size) const { IndexBufferObject::create_primitive(size * sizeof(T)); }

        static IndexBufferObjectTyped cast(IndexBufferObject &&move) { return IndexBufferObjectTyped(std::move(move)); }

    protected:
    };
}// namespace render

#endif// BONGOMOUNTAIN_INDEXBUFFEROBJECT_HPP
