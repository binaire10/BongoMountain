#ifndef GAMEENGINEBONGO_TYPE_TRAITS_HPP
#define GAMEENGINEBONGO_TYPE_TRAITS_HPP

#include <type_traits>

namespace Core {
    template<template<typename...> typename, template<typename...> typename>
    struct is_same_template : std::false_type {};
    template<template<typename...> typename T>
    struct is_same_template<T, T> : std::true_type {};

    template<template<typename...> typename T1, template<typename...> typename T2>
    constexpr bool is_same_template_v = is_same_template<T1, T2>::value;
}

#endif// GAMEENGINEBONGO_TYPE_TRAITS_HPP
