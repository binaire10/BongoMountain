#ifndef BONGOMOUNTAIN_TYPE_STORAGE_HPP
#define BONGOMOUNTAIN_TYPE_STORAGE_HPP

#include <type_traits>
#include <utility>

#include "type_traits.hpp"

namespace core {
    template<typename... T>
    struct SetKind;

    template<typename, typename>
    struct merge_type;

    template<typename... K, typename... X>
    struct merge_type<SetKind<K...>, SetKind<X...>> {
        using type = SetKind<K..., X...>;
    };

    template<typename T, auto data, typename I = std::make_index_sequence<data.size()>>
    struct filter_type;
    template<typename... T, auto data, std::size_t... idx>
    struct filter_type<SetKind<T...>, data, std::index_sequence<idx...>> {
        using type = SetKind<std::tuple_element_t<data[idx], std::tuple<T...>>...>;
    };
     
    template<typename... T>
    struct SetKind {
        template<typename K>
        struct contains : std::integral_constant<bool, (... || std::is_same_v<K, T>)> {};

        template<typename K>
        static constexpr bool contains_v = contains<K>::value;

        static constexpr std::index_sequence_for<T...> getSequence() {return {};}

        template<typename K>
        static constexpr std::size_t indexOf() {
            return find_index<K, T...>();
        }

        template<typename... K>
        using merge = decltype((std::declval<SetKind>() | ... | std::declval<K>()));

        template<typename... K>
        using add = decltype((std::declval<SetKind>() << ... << std::declval<K>()));
        //using add = merge<SetKind<K>...>;

        template<template<typename...> class X>
        using make = X<T...>;

        template<typename K>
        using base_of = typename filter_type<SetKind<T...>, generateIndex<std::array<bool, sizeof...(T)>{std::is_base_of_v<T, K>...}>()>::type;

        template<typename K>
        using instance_of = typename filter_type<SetKind<T...>, generateIndex<std::array<bool, sizeof...(T)>{std::is_base_of_v<K, T>...}>()>::type;

        template<typename K>
        using add_type = std::conditional_t<(contains_v<K>), SetKind<T...>, SetKind<T..., K>>;

        template<typename K>
        auto operator<<(const K &) -> add_type<K> {
            return {};
        }

        template<typename... K>
        auto operator|(SetKind<K...>) -> add<K...> {
            return {};
        }
    };

        template<auto val>
    struct ValueHolder {
        static constexpr auto value = val;
        static constexpr bool is_function = false;
        static constexpr bool is_method = false;
    };
    
    template<typename Ret, typename... Arg, Ret(*val)(Arg...)>
    struct ValueHolder<val> {
        using return_type = Ret;
        using argument_types = std::tuple<std::decay_t<Arg>...>;
        static constexpr auto value = val;
        static constexpr bool is_function = true;
        static constexpr bool is_method = false;
    };

    template<typename Ret, typename Holder, typename... Arg, Ret(Holder::*val)(Arg...)>
    struct ValueHolder<val> {
        using return_type = Ret;
        using object_type = Holder;
        using argument_types = std::tuple<std::decay_t<Arg>...>;
        static constexpr auto value = val;
        static constexpr bool is_function = false;
        static constexpr bool is_method = true;
    };

    template<typename Ret, typename Holder, typename... Arg, Ret(Holder::*val)(Arg...) &>
    struct ValueHolder<val> {
        using return_type = Ret;
        using object_type = Holder;
        using argument_types = std::tuple<std::decay_t<Arg>...>;
        static constexpr auto value = val;
        static constexpr bool is_function = false;
        static constexpr bool is_method = true;
    };

    template<typename Ret, typename Holder, typename... Arg, Ret(Holder::*val)(Arg...) &&>
    struct ValueHolder<val> {
        using return_type = Ret;
        using object_type = Holder;
        using argument_types = std::tuple<std::decay_t<Arg>...>;
        static constexpr auto value = val;
        static constexpr bool is_function = false;
        static constexpr bool is_method = true;
    };

    template<typename Ret, typename Holder, typename... Arg, Ret(Holder::*val)(Arg...) const>
    struct ValueHolder<val> {
        using return_type = Ret;
        using object_type = Holder;
        using argument_types = std::tuple<std::decay_t<Arg>...>;
        static constexpr auto value = val;
        static constexpr bool is_function = false;
        static constexpr bool is_method = true;
    };

    template<typename Ret, typename Holder, typename... Arg, Ret(Holder::*val)(Arg...) const &>
    struct ValueHolder<val> {
        using return_type = Ret;
        using object_type = Holder;
        using argument_types = std::tuple<std::decay_t<Arg>...>;
        static constexpr auto value = val;
        static constexpr bool is_function = false;
        static constexpr bool is_method = true;
    };

    template<typename Ret, typename Holder, typename... Arg, Ret(Holder::*val)(Arg...) const &&>
    struct ValueHolder<val> {
        using return_type = Ret;
        using object_type = Holder;
        using argument_types = std::tuple<std::decay_t<Arg>...>;
        static constexpr auto value = val;
        static constexpr bool is_function = false;
        static constexpr bool is_method = true;
    };

    template<typename Ret, typename Holder, typename... Arg, Ret(Holder::*val)(Arg...) noexcept>
    struct ValueHolder<val> {
        using return_type = Ret;
        using object_type = Holder;
        using argument_types = std::tuple<std::decay_t<Arg>...>;
        static constexpr auto value = val;
        static constexpr bool is_function = false;
        static constexpr bool is_method = true;
    };

    template<typename Ret, typename Holder, typename... Arg, Ret(Holder::*val)(Arg...) & noexcept>
    struct ValueHolder<val> {
        using return_type = Ret;
        using object_type = Holder;
        using argument_types = std::tuple<std::decay_t<Arg>...>;
        static constexpr auto value = val;
        static constexpr bool is_function = false;
        static constexpr bool is_method = true;
    };

    template<typename Ret, typename Holder, typename... Arg, Ret(Holder::*val)(Arg...) && noexcept>
    struct ValueHolder<val> {
        using return_type = Ret;
        using object_type = Holder;
        using argument_types = std::tuple<std::decay_t<Arg>...>;
        static constexpr auto value = val;
        static constexpr bool is_function = false;
        static constexpr bool is_method = true;
    };

    template<typename Ret, typename Holder, typename... Arg, Ret(Holder::*val)(Arg...) const noexcept>
    struct ValueHolder<val> {
        using return_type = Ret;
        using object_type = Holder;
        using argument_types = std::tuple<std::decay_t<Arg>...>;
        static constexpr auto value = val;
        static constexpr bool is_function = false;
        static constexpr bool is_method = true;
    };

    template<typename Ret, typename Holder, typename... Arg, Ret(Holder::*val)(Arg...) const & noexcept>
    struct ValueHolder<val> {
        using return_type = Ret;
        using object_type = Holder;
        using argument_types = std::tuple<std::decay_t<Arg>...>;
        static constexpr auto value = val;
        static constexpr bool is_function = false;
        static constexpr bool is_method = true;
    };

    template<typename Ret, typename Holder, typename... Arg, Ret(Holder::*val)(Arg...) const && noexcept>
    struct ValueHolder<val> {
        using return_type = Ret;
        using object_type = Holder;
        using argument_types = std::tuple<std::decay_t<Arg>...>;
        static constexpr auto value = val;
        static constexpr bool is_function = false;
        static constexpr bool is_method = true;
    };
} // namespace core

#endif // BONGOMOUNTAIN_TYPE_STORAGE_HPP
