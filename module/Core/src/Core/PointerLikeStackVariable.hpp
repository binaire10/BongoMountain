#ifndef GAMEENGINEBONGO_POINTERLIKESTACKVARIABLE_HPP
#define GAMEENGINEBONGO_POINTERLIKESTACKVARIABLE_HPP

template<typename T>
class PointerLikeStackVariable : T
{
public:
    [[nodiscard]] T *operator->() noexcept { return this; }

    [[nodiscard]] T &operator*() noexcept { return *this; }
};


#endif// GAMEENGINEBONGO_POINTERLIKESTACKVARIABLE_HPP
