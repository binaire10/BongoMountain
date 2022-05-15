#ifndef BONGOMOUNTAIN_TYPE_TRAITS_HPP
#define BONGOMOUNTAIN_TYPE_TRAITS_HPP

#include <type_traits>

namespace core {
    template<template<typename...> typename, template<typename...> typename>
    struct is_same_template : std::false_type {};
    template<template<typename...> typename T>
    struct is_same_template<T, T> : std::true_type {};

    template<template<typename...> typename T1, template<typename...> typename T2>
    constexpr bool is_same_template_v = is_same_template<T1, T2>::value;

    template<bool condition, template<typename...> typename T>
    struct enable_template_if {};

    template<template<typename...> typename T>
    struct enable_template_if<true, T> {
        template<typename... ArgT>
        using type = T<ArgT...>;
    };

    template<bool condition, template<typename...> typename T>
    using enable_template_if_t = typename enable_template_if<condition, T>::type;

    template<typename SignatureT>
    constexpr SignatureT *get_overload(SignatureT *signature) {
        return signature;
    }
    template<typename SignatureT, typename ObjectT>
    constexpr auto get_overload(SignatureT (ObjectT::*signature)) -> SignatureT(ObjectT::*) {
        return signature;
    }
}

#endif// BONGOMOUNTAIN_TYPE_TRAITS_HPP
