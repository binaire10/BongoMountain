#ifndef GAMEENGINEBONGO_BUFFERVECTOR_HPP
#define GAMEENGINEBONGO_BUFFERVECTOR_HPP

#include <Core/Assert.hpp>
#include "BufferArray.hpp"
#include "detail/Traits.hpp"

namespace Graphic
{
    namespace detail
    {
        template<class T, typename TraitT>
        class BufferVector
        {
        public:
            using container_type = typename TraitT::container_type;

            [[nodiscard]] const container_type &getContainer() const { return container; }
            [[nodiscard]] container_type       &getContainer() { return container; }
            [[nodiscard]] std::size_t           getCount() const { return count; }
            [[nodiscard]] std::size_t           getCapacity() const { return capacity; }


            [[nodiscard]] static BufferVector create(std::size_t len)
            {
                BufferVector buffer;
                buffer.container = TraitT::create(len);
                buffer.count     = 0;
                buffer.capacity  = len;
                return buffer;
            }

            template<std::size_t len>
            [[nodiscard]] static BufferVector create(const T (&data)[len])
            {
                BufferVector buffer;
                buffer.container = TraitT::create(data);
                buffer.count     = len;
                buffer.capacity  = len;
                return buffer;
            }

            void remove(std::size_t offset, std::size_t count)
            {
                BM_CORE_ASSERT((offset + count) <= BufferVector::count, "remove outside of buffer element");
                std::size_t overflow = BufferVector::count - (offset + count);
                if(overflow > 0)
                    TraitT::memcpy(container, offset, container, offset + count, overflow);
                BufferVector::count -= count;
            }

            void insert_back(const T *data, std::size_t count)
            {
                BM_CORE_ASSERT((BufferVector::count + count) < BufferVector::capacity, "overflow capacity");
                TraitT::write(container, 0, data, count);
                BufferVector::count += count;
            }

            void set(std::size_t offset, const T *data, std::size_t count)
            {
                BM_CORE_ASSERT((offset + count) <= BufferVector::count, "overflow count of used element");
                TraitT::write(container, offset, data, count);
            }

            void set(std::size_t offset, const T &data)
            {
                BM_CORE_ASSERT((offset + 1) <= BufferVector::count, "overflow count of used element");
                TraitT::write(container, offset, &data, 1);
            }

        private:
            container_type container;
            std::size_t    count;
            std::size_t    capacity;
        };
    }// namespace detail


    template<typename T>
    using VertexBufferVector = detail::BufferVector<T, detail::container_trait<T, Graphic::VertexBuffer>>;
    template<typename T>
    using IndexBufferVector = detail::BufferVector<T, detail::container_trait<T, Graphic::IndexBuffer>>;

    template<typename T>
    using SharedVertexBufferVector = detail::BufferVector<T, detail::container_trait<T, Graphic::SharedVertexBuffer>>;
    template<typename T>
    using SharedIndexBufferVector = detail::BufferVector<T, detail::container_trait<T, Graphic::SharedIndexBuffer>>;
}// namespace Graphic

#endif// GAMEENGINEBONGO_BUFFERVECTOR_HPP
