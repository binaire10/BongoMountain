#ifndef BONGOMOUNTAIN_BUFFER_HPP
#define BONGOMOUNTAIN_BUFFER_HPP

#include <algorithm>
#include <memory>
#include <span>
#include <array>
#include <limits>
#include <type_traits>
#include <cassert>
#include <vector>

namespace core
{
    template<std::size_t extent = std::dynamic_extent, bool is_const = false>
    class ByteTransaction
    {
    public:
        using size_type      = std::uint64_t;
        using storage_type   = std::span<std::conditional_t<is_const, const std::byte, std::byte>, extent>;
        using pointer        = typename storage_type::pointer;
        using const_pointer  = typename storage_type::const_pointer;
        using iterator       = typename storage_type::iterator;
    private:
        storage_type storage;
        iterator     pos = storage.begin();

    public:
        ByteTransaction(storage_type view)
            : storage(view)
        {}

        template<typename T, std::size_t ex = std::dynamic_extent>
            requires (std::is_trivial_v<T> and not is_const)
        explicit ByteTransaction(std::span<T, ex> view)
            : ByteTransaction(std::as_writable_bytes(view))
        {}

                template<typename T, std::size_t ex = std::dynamic_extent>
                    requires (std::is_trivial_v<T> and is_const)
                explicit ByteTransaction(std::span<T, ex> view)
                    : ByteTransaction(std::as_bytes(view))
                {}

        template<typename T>
            requires std::is_trivial_v<T>
        ByteTransaction(T &value)
            : ByteTransaction(std::span<T, 1>{ &value, 1 })
        {}

        template<typename T>
            requires(!std::is_trivial_v<T>)
        ByteTransaction(T &value)
            : ByteTransaction(
                  std::span<std::decay_t<decltype(*std::begin(value))>>(std::begin(value), std::size(value)))
        {}

        void consume(std::size_t bytes)
        {
            bytes = std::min(bytes, size_bytes());
            pos   = std::copy(std::next(storage.begin(), std::ptrdiff_t(bytes)), end(), storage.begin());
        }

        std::size_t write(const void *data, std::size_t size) noexcept
        {
            const std::size_t count = std::min(size, available_bytes());
            pos                     = std::copy_n(reinterpret_cast<const_pointer>(data), count, begin());
            return count;
        }

        void clear() { pos = storage.begin(); }

        bool empty() const noexcept { return std::to_address(begin()) == data(); }

        explicit operator bool() const { return full(); }

        bool operator!() const { return !full(); }

        bool full() const noexcept { return begin() == end(); }

        std::size_t size_bytes() const noexcept { return std::size_t(std::distance(storage.begin(), pos)); }

        constexpr std::size_t capacity_bytes() const noexcept { return storage.size(); }

        std::size_t available_bytes() const noexcept { return std::size_t(std::distance(begin(), end())); }

        pointer position() & noexcept { return std::to_address(begin()); }

        pointer data() & noexcept { return storage.data(); }

        const_pointer position() const & noexcept { return std::to_address(begin()); }

        const_pointer data() const & noexcept { return storage.data(); }

        const_pointer data() const && noexcept     = delete;
        const_pointer position() const && noexcept = delete;

        iterator begin() & { return pos; }

        iterator end() & { return storage.end(); }

        iterator begin() const & { return pos; }

        iterator end() const & { return storage.end(); }

        size_type accept(iterator endPosition) &
        {
            const auto byteCount = size_type(std::distance(begin(), endPosition));
            std::advance(pos, std::ptrdiff_t(byteCount));
            return byteCount;
        }

        ssize_t accept(ssize_t bytes) &
        {
            if(bytes > 0)
            {
                assert(bytes <= available_bytes());
                std::advance(pos, std::ptrdiff_t(bytes));
            }
            return bytes;
        }

        ssize_t set_bytes(ssize_t bytes) &
        {
            if(bytes > 0)
            {
                assert(bytes <= capacity_bytes());
                pos = std::next(storage.begin(), std::ptrdiff_t(bytes));
            }
            return bytes;
        }
    };

    template<typename T>
        requires std::is_trivial_v<T>
    ByteTransaction(T &) -> ByteTransaction<sizeof(T)>;

    template<typename T, std::size_t ex>
        requires std::is_trivial_v<T>
    ByteTransaction(std::span<T, ex>)
        -> ByteTransaction<ex == std::dynamic_extent ? std::dynamic_extent : ex * sizeof(T)>;

    template<typename T, std::size_t Extend = std::dynamic_extent>
        requires(std::is_trivial_v<T>)
    class ByteBuffer
    {
    public:
        using size_type = std::conditional_t<
            Extend == std::dynamic_extent,
            std::uint64_t,
            std::conditional_t<
                Extend <= std::numeric_limits<std::uint8_t>::max(),
                std::uint8_t,
                std::conditional_t<Extend <= std::numeric_limits<std::uint16_t>::max(),
                                   std::uint16_t,
                                   std::conditional_t<Extend <= std::numeric_limits<std::uint32_t>::max(),
                                                      std::uint32_t,
                                                      std::uint64_t>>>>;

        using storage_type   = std::conditional_t<Extend == std::dynamic_extent, std::vector<T>, std::array<T, Extend>>;
        using pointer        = typename storage_type::pointer;
        using const_pointer  = typename storage_type::const_pointer;
        using iterator       = typename storage_type::iterator;
        using const_iterator = typename storage_type::const_iterator;

        ByteBuffer() = default;

        template<typename SizeT = std::size_t>
            requires(Extend == std::dynamic_extent)
        ByteBuffer(SizeT size)
        {
            if constexpr(Extend == std::dynamic_extent)
            {
                buffer.resize(size);
            }
        }

        template<typename SizeT = std::size_t>
            requires(Extend == std::dynamic_extent)
        void resize(SizeT size)
        {
            if constexpr(Extend == std::dynamic_extent)
            {
                buffer.resize(size);
            }
        }

        void consume(std::size_t bytes)
        {
            bytes = std::min(bytes, size());
            std::copy(std::next(buffer.begin(), std::ptrdiff_t(bytes)), end(), buffer.begin());
            pos -= bytes;
        }

        std::size_t write(const void *data, std::size_t size) noexcept
        {
            std::size_t count = std::min(size, available());
            std::copy_n(reinterpret_cast<const T *>(data), count, position());
            pos += count;
            return count;
        }

        explicit operator bool() const { return full(); }

        bool operator!() const { return !full(); }

        const_iterator begin() const && = delete;
        const_iterator end() const &&   = delete;

        iterator begin() & { return buffer.begin(); }

        iterator end() & { return std::next(begin(), std::ptrdiff_t(pos)); }

        const_iterator begin() const & { return buffer.begin(); }

        const_iterator end() const & { return std::next(begin(), std::ptrdiff_t(pos)); }

        void clear() { pos = 0; }

        bool empty() const noexcept { return size() == 0; }

        bool full() const noexcept { return size() == capacity(); }

        std::span<const T> view() const && noexcept = delete;
        std::span<const T> view() const & noexcept { return std::span<const T>{ begin(), end() }; }

        pointer position() & noexcept { return std::next(data(), pos); }

        pointer data() & noexcept { return buffer.data(); }

        const_pointer position() const & noexcept { return std::next(data(), pos); }

        const_pointer data() const & noexcept { return buffer.data(); }

        const_pointer data() const && noexcept     = delete;
        const_pointer position() const && noexcept = delete;

        std::size_t size() const noexcept { return std::size_t(pos); }

        std::size_t capacity() const noexcept { return buffer.size(); }

        std::size_t available() const noexcept { return capacity() - size(); }

        ssize_t accept(ssize_t bytes) &
        {
            if(bytes > 0)
            {
                assert(bytes <= available());
                pos += size_type(bytes);
            }
            return bytes;
        }

        ssize_t set_bytes(ssize_t bytes) &
        {
            if(bytes > 0)
            {
                assert(bytes <= capacity());
                pos = size_type(bytes);
            }
            else
            {
                pos = size_type(0);
            }
            return bytes;
        }

    private:
        storage_type buffer;
        size_type    pos = 0;
    };
}// namespace core

#endif// BONGOMOUNTAIN_BUFFER_HPP