#ifndef GAMEENGINEBONGO_BUFFERARRAY_HPP
#define GAMEENGINEBONGO_BUFFERARRAY_HPP

#include <cstdint>
#include "BufferObject.hpp"
#include "detail/Traits.hpp"

template<class T, typename TraitT>
class BufferArray : public TraitT::container_type
{
public:
    using base_type = typename TraitT::container_type;
    BufferArray(BufferArray &&) noexcept = default;

    template<std::size_t n>
    void set(std::size_t offset, const T (&data)[n])
    {
        TraitT::write(*this, offset, data, n);
    }

    void set(std::size_t offset, const T *data, std::size_t n) { TraitT::write(this, offset, data, n); }

    [[nodiscard]] std::size_t getCount() const { return count; }


    static BufferArray create(std::size_t len) {
        BufferArray buffer = TraitT::create(len);
        buffer.count = len;
        return buffer;
    }

    template<std::size_t len>
    static BufferArray create(const T (&data)[len]) {
        BufferArray buffer = TraitT::create(data);
        buffer.count = len;
        return buffer;
    }

protected:
    BufferArray(base_type &&base) noexcept : base_type{std::move(base)} {}

private:
    std::size_t count = 0;
};

namespace Graphic {
    template<typename T>
    using VertexBufferArray = BufferArray<T, detail::container_trait<T, Graphic::VertexBuffer>>;
    template<typename T>
    using IndexBufferArray = BufferArray<T, detail::container_trait<T, Graphic::IndexBuffer>>;

    template<typename T>
    using SharedVertexBufferArray = BufferArray<T, detail::container_trait<T, Graphic::SharedVertexBuffer>>;
    template<typename T>
    using SharedIndexBufferArray = BufferArray<T, detail::container_trait<T, Graphic::SharedIndexBuffer>>;

    template<typename T>
    void copy(const VertexBufferArray<T> &src, std::size_t soffset,
              VertexBufferArray<T> &dest, std::size_t doffset, std::size_t count) {
        memcpy(src, soffset * sizeof(T), dest, doffset * sizeof(T), count * sizeof(T));
    }

    template<typename T>
    void copy(const IndexBufferArray<T> &src, std::size_t soffset,
              IndexBufferArray<T> &dest, std::size_t doffset, std::size_t count) {
        memcpy(src, soffset * sizeof(T), dest, doffset * sizeof(T), count * sizeof(T));
    }
    template<typename T>
    void copy(const SharedVertexBufferArray<T> &src, std::size_t soffset,
              SharedVertexBufferArray<T> &dest, std::size_t doffset, std::size_t count) {
        memcpy(src, soffset * sizeof(T), dest, doffset * sizeof(T), count * sizeof(T));
    }

    template<typename T>
    void copy(const SharedIndexBufferArray<T> &src, std::size_t soffset,
              SharedIndexBufferArray<T> &dest, std::size_t doffset, std::size_t count) {
        memcpy(src, soffset * sizeof(T), dest, doffset * sizeof(T), count * sizeof(T));
    }
}

#endif// GAMEENGINEBONGO_BUFFERARRAY_HPP
