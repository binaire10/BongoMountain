#ifndef BONGOMOUNTAIN_VERTEXBUFFEROBJECT_HPP
#define BONGOMOUNTAIN_VERTEXBUFFEROBJECT_HPP

#include <memory>
#include "../graphic/config.hpp"
#include <core/Assert.hpp>
#include "RenderElement.hpp"
#include "private/VertexBufferPrivate.hpp"

namespace render
{
    class BM_GRAPHIC_DCL VertexBufferObject final : public RenderElement
    {
    public:
        VertexBufferObject() = default;
        VertexBufferObject(RenderDevice *owner, std::unique_ptr<detail::VertexBufferPrivate> &&handler);
        ~VertexBufferObject() override;

        VertexBufferObject(VertexBufferObject &&) noexcept            = default;
        VertexBufferObject &operator=(VertexBufferObject &&) noexcept = default;

        void bind() const;
        void create(std::size_t size);
        void create(const void *data, std::size_t size);
        void overwrite(std::size_t offset, const void *data, std::size_t size);
        template<typename T>
        void overwrite(std::size_t offset, const T *data, std::size_t size)
        {
            m_handler->overwrite(offset, reinterpret_cast<const void *>(data), size * sizeof(T));
        }
        template<typename T, std::size_t size>
        void overwrite(std::size_t offset, const T (&data)[size])
        {
            m_handler->overwrite(offset, reinterpret_cast<const void *>(data), size * sizeof(T));
        }
        void insert(std::size_t offset, const void *data, std::size_t size);
        template<class T>
        void insert(std::size_t offset, const T *data, std::size_t size)
        {
            insert(offset, reinterpret_cast<const void *>(data), size * sizeof(T));
        }
        template<class T, std::size_t size>
        void insert(std::size_t offset, const T (&data)[size])
        {
            insert(offset, reinterpret_cast<const void *>(data), size * sizeof(T));
        }
        void erase(std::size_t offset, std::size_t size);
        void clear();
        void destroy();

        [[nodiscard]] std::size_t                                                   size() const;
        [[nodiscard]] constexpr const std::unique_ptr<detail::VertexBufferPrivate> &getPrivate() const
        {
            return m_handler;
        }

    private:
        std::unique_ptr<detail::VertexBufferPrivate> m_handler;
    };
}// namespace render

#endif// BONGOMOUNTAIN_VERTEXBUFFEROBJECT_HPP
