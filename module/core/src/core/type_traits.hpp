#ifndef BONGOMOUNTAIN_TYPE_TRAITS_HPP
#define BONGOMOUNTAIN_TYPE_TRAITS_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
#include <memory>
#include <tuple>
#include <type_traits>

namespace core
{
    template<template<typename...> typename, typename T>
    struct is_template_kind : std::false_type
    {};

    template<template<typename...> typename K, typename... T>
    struct is_template_kind<K, K<T...>> : std::true_type
    {};

    template<template<typename...> typename T1, typename T2>
    constexpr bool is_template_kind_v = is_template_kind<T1, T2>::value;

    template<template<typename...> typename, template<typename...> typename>
    struct is_same_template : std::false_type
    {};
    template<template<typename...> typename T>
    struct is_same_template<T, T> : std::true_type
    {};

    template<template<typename...> typename T1, template<typename...> typename T2>
    constexpr bool is_same_template_v = is_same_template<T1, T2>::value;

    template<bool condition, template<typename...> typename T>
    struct enable_template_if
    {};

    template<template<typename...> typename T>
    struct enable_template_if<true, T>
    {
        template<typename... ArgT>
        using type = T<ArgT...>;
    };

    template<bool condition, template<typename...> typename T>
    using enable_template_if_t = typename enable_template_if<condition, T>::type;

    template<typename SignatureT>
    constexpr SignatureT *get_overload(SignatureT *signature)
    {
        return signature;
    }

    template<typename SignatureT, typename ObjectT>
    constexpr auto get_overload(SignatureT(ObjectT::*signature)) -> SignatureT(ObjectT::*)
    {
        return signature;
    }

    template<typename T>
    struct is_unique_ptr : is_template_kind<std::unique_ptr, T>
    {};

    template<typename T>
    constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;

    template<typename T>
    struct is_shared_ptr : is_template_kind<std::shared_ptr, T>
    {};

    template<typename T>
    constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

    template<typename K, bool = core::is_unique_ptr_v<K> || core::is_shared_ptr_v<K>>
    struct underlying_ptr_type
    {
        using type = std::remove_pointer_t<K>;
    };

    template<typename K>
    struct underlying_ptr_type<K, true>
    {
        using type = typename K::element_type;
    };

    template<typename K>
    using underlying_ptr_t = typename underlying_ptr_type<K>::type;

    template<typename T>
    using interface_t = typename T::interface_type;

    template<typename T>
    struct FunctionDecompose
    {
        using argument_type = std::tuple<>;
    };

    template<typename R, typename... ArgT>
    struct FunctionDecompose<R(ArgT...)>
    {
        using result_type   = R;
        using argument_type = std::tuple<ArgT...>;
        template<template<typename... K> class X>
        using make_argument = X<ArgT...>;
    };

    template<typename R, typename... ArgT>
    struct FunctionDecompose<R (*)(ArgT...)>
    {
        using result_type   = R;
        using argument_type = std::tuple<ArgT...>;
        template<template<typename... K> class X>
        using make_argument = X<ArgT...>;
    };

    template<typename R, typename ObjectT, typename... ArgT>
    struct FunctionDecompose<R (ObjectT::*)(ArgT...)>
    {
        using result_type   = R;
        using object_type   = ObjectT;
        using argument_type = std::tuple<ArgT...>;
        template<template<typename... K> class X>
        using make_argument = X<ArgT...>;
    };

    template<typename R, typename ObjectT, typename... ArgT>
    struct FunctionDecompose<R (ObjectT::*)(ArgT...) const>
    {
        using result_type   = R;
        using object_type   = ObjectT;
        using argument_type = std::tuple<ArgT...>;
        template<template<typename... K> class X>
        using make_argument = X<ArgT...>;
    };

    template<std::size_t N>
    constexpr std::size_t countTrue(const std::array<bool, N> &array)
    {
        return std::size_t(std::count(array.begin(), array.end(), true));
    }

    template<std::size_t N>
    constexpr std::size_t countFalse(const std::array<bool, N> &array)
    {
        return std::size_t(std::count(array.begin(), array.end(), false));
    }

    template<auto data>
    constexpr std::array<std::size_t, countTrue(data)> generateIndex()
    {
        std::array<std::size_t, countTrue(data)> result;
        auto                                     iter = result.begin();
        std::size_t                              idx{};
        for(bool idb : data)
        {
            if(idb)
            {
                *(iter++) = idx;
            }
            ++idx;
        }
        return result;
    }

    template<template<typename x1> class Y, typename... X>
    constexpr std::array<bool, sizeof...(X)> test_type()
    {
        return std::array<bool, sizeof...(X)>{ Y<X>::value... };
    }

    template<std::size_t N>
    constexpr std::array<bool, N> negate(const std::array<bool, N> &data)
    {
        std::array<bool, N> result;
        auto                iter = result.begin();
        for(bool idb : data) { *(iter++) = !idb; }
        return result;
    }

    template<template<typename x1, typename x2> class K, typename T, typename... X>
    constexpr std::array<bool, sizeof...(X)> detect_index()
    {
        constexpr std::array<bool, sizeof...(X)> find_interface{ K<T, X>::value... };
        return find_interface;
    }

    template<typename T, typename... X>
    constexpr bool contains_type()
    {
        return countTrue(detect_index<std::is_same, T, X...>()) != 0;
    }

    template<typename T, typename... X>
    constexpr std::ptrdiff_t find_index()
    {
        constexpr std::array<bool, sizeof...(X)> find_interface = detect_index<std::is_same, T, X...>();
        return std::distance(find_interface.begin(), std::find(find_interface.begin(), find_interface.end(), true));
    }

    template<template<typename x1, typename x2> class K, typename T, typename... X>
    constexpr std::size_t count_index()
    {
        constexpr std::array<bool, sizeof...(X)> find_interface = detect_index<K, T, X...>();
        return std::size_t(std::count(find_interface.begin(), find_interface.end(), true));
    }

    template<template<typename x1, typename x2> class K, typename T, typename... X>
    constexpr std::array<std::size_t, count_index<K, T, X...>()> get_indexes()
    {
        const std::array<bool, sizeof...(X)>               find_interface = detect_index<K, T, X...>();
        std::array<std::size_t, count_index<K, T, X...>()> result;
        auto                                               iter = result.begin();
        std::size_t                                        idx{};
        for(bool idb : find_interface)
        {
            if(idb)
            {
                *(iter++) = idx;
            }
            ++idx;
        }
        return result;
    }
}// namespace core

#endif// BONGOMOUNTAIN_TYPE_TRAITS_HPP
