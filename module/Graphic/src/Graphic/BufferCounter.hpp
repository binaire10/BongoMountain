#ifndef GAMEENGINEBONGO_BUFFERCOUNTER_HPP
#define GAMEENGINEBONGO_BUFFERCOUNTER_HPP

#include <cstdint>

template<class T, typename Base>
class BufferCounter : public Base
{
public:
    template<std::size_t n>
    void create(const T (&data)[n])
    {
        create(data, n);
    }

    void create(const T *data, std::size_t n)
    {
        count = n;
        Base::create(data, n * sizeof(T));
    }

    [[nodiscard]] std::size_t getCount() const { return count; }

private:
    std::size_t count = 0;
};

#endif// GAMEENGINEBONGO_BUFFERCOUNTER_HPP
