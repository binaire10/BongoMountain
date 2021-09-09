#ifndef GAMEENGINEBONGO_BUFFERARRAY_HPP
#define GAMEENGINEBONGO_BUFFERARRAY_HPP

#include <cstdint>

template<class T, typename Base>
class BufferArray : public Base
{
public:
    BufferArray(BufferArray &&) noexcept = default;
    void allocate(std::size_t n)
    {
        count = n;
        Base::allocate(n * sizeof(T));
    }

    template<std::size_t n>
    void write(std::size_t offset, const T (&data)[n])
    {
        Base::write(offset, data, n * sizeof(T));
    }

    void write(std::size_t offset, const T *data, std::size_t n) { Base::write(offset, data, n * sizeof(T)); }

    [[nodiscard]] std::size_t getCount() const { return count; }


    static BufferArray create(std::size_t len) {
        BufferArray buffer = Base::template create<T>(len);
        buffer.count = len;
        return buffer;
    }

    template<std::size_t len>
    static BufferArray create(const T (&data)[len]) {
        BufferArray buffer = Base::create(data);
        buffer.count = len;
        return buffer;
    }

protected:
    BufferArray(Base &&base) noexcept : Base{std::move(base)} {}

private:
    std::size_t count = 0;
};

#endif// GAMEENGINEBONGO_BUFFERARRAY_HPP
