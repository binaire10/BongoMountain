#ifndef GAMEENGINEBONGO_MEMORY_HPP
#define GAMEENGINEBONGO_MEMORY_HPP

#ifdef _MSC_BUILD
#    include <malloc.h>
#    include <Core/Assert.hpp>

namespace
{
    template<typename TypeT>
    requires std::is_nothrow_destructible_v<TypeT> && std::is_nothrow_constructible_v<TypeT>
    struct safe_alloca_guard
    {
        safe_alloca_guard(const safe_alloca_guard &)     = delete;
        safe_alloca_guard(safe_alloca_guard &&) noexcept = delete;

        safe_alloca_guard(TypeT *s, std::size_t count) noexcept
            : data{ s }
            , count{ count }
        {
            std::uninitialized_default_construct_n(data, count);
        }

        ~safe_alloca_guard() noexcept
        {
            std::destroy_n(data, count);
            _freea(data);
        }

        TypeT *     data;
        std::size_t count;
    };
}// namespace
#    define BM_STACK_ARRAY(T, name, space)                                                  \
        safe_alloca_guard<T> name##__LINE__{ static_cast<T *>(_malloca(space * sizeof(T))), \
                                             static_cast<std::size_t>(space) };             \
        T *const             name = name##__LINE__.data
#else
#    define BM_STACK_ARRAY(T, name, space) \
        T        name##__LINE__[space];    \
        T *const name = name##__LINE__
#endif

template<typename Callback, typename CharT, typename TraitT>
auto fixe_string(std::basic_string_view<CharT, TraitT> str, Callback f) {
    if(!TraitT::eq(*(str.data() + str.size()), TraitT::to_char_type(0))) {
        BM_STACK_ARRAY(CharT, fixed, str.size() + 1);
        *std::copy(str.begin(), str.end(), fixed) = TraitT::to_char_type(0);
        return f(std::basic_string_view<CharT, TraitT>{fixed, str.size()});
    }
    return f(str);
}

#endif// GAMEENGINEBONGO_MEMORY_HPP
