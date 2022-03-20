#ifndef BONGOMOUNTAIN_RESSOURCE_HPP
#define BONGOMOUNTAIN_RESSOURCE_HPP

#include <utility>
#include <atomic>
#include <type_traits>

namespace core
{
    template<typename T, typename TraitT>
    class resource_view
    {
    public:
        constexpr explicit resource_view(const T &data)
            : data{ data }
        {}

        [[nodiscard]] constexpr bool isValid() const noexcept { return data != TraitT::invalid_resource(); }

        constexpr explicit operator bool() const noexcept { return isValid(); }

        [[nodiscard]] constexpr bool operator!() const { return !isValid(); }

        constexpr const T &getResource() const noexcept { return data; }

    private:
        T data;
    };

    template<typename T, typename TraitT>
    class resource : private TraitT
    {
    public:
        using trait_type = TraitT;
        explicit constexpr resource(T value = TraitT::invalid_resource(), TraitT trait = {}) noexcept(
            std::is_nothrow_move_constructible_v<TraitT> and std::is_nothrow_move_constructible_v<T>)
            : TraitT{ std::move(trait) }
            , data{ std::move(value) }
        {}
        resource(const resource &) = delete;
        resource &operator=(const resource &) = delete;
        constexpr resource(resource &&move) noexcept
            : data{ std::move(move.data) }
        {
            move.data = TraitT::invalid_resource();
        }

        [[nodiscard]] constexpr bool isValid() const noexcept { return data != TraitT::invalid_resource(); }

        constexpr explicit operator bool() const noexcept { return isValid(); }
        constexpr          operator resource_view<T, TraitT>() const noexcept { return resource_view<T, TraitT>(data); }

        [[nodiscard]] constexpr bool operator!() const { return data == TraitT::invalid_resource(); }

        resource &operator=(resource &&move) noexcept
        {
            if(this == &move)
                return *this;
            data      = std::move(move.data);
            move.data = TraitT::invalid_resource();
            return *this;
        }

        ~resource() noexcept(std::is_nothrow_destructible_v<T> and std::is_nothrow_destructible_v<TraitT> and noexcept(
            std::declval<TraitT>().destroy(std::declval<T &>())))
        {
            if(data != TraitT::invalid_resource())
                TraitT::destroy(data);
        }

        template<typename... ArgT>
        void create(ArgT... arg) noexcept(noexcept(std::declval<TraitT>().create(std::forward<ArgT>(arg)...)))
        {
            data = TraitT::create(std::forward<ArgT>(arg)...);
        }

        void destroy() noexcept(noexcept(std::declval<TraitT>().destroy(std::declval<T &>())))
        {
            if(data != TraitT::invalid_resource())
            {
                TraitT::destroy(data);
                data = TraitT::invalid_resource();
            }
        }

        constexpr const T &getResource() const noexcept { return data; }

        T release() noexcept
        {
            T tmp = std::move(data);
            data  = TraitT::invalid_resource();
            return tmp;
        }

        TraitT &getTrait() noexcept { return *this; }

    private:
        T data;
    };

    template<typename T, typename TraitT>
    class shared_resource : private TraitT
    {
    public:
        using trait_type = TraitT;
        explicit constexpr shared_resource(resource<T, TraitT> &&promote) noexcept(
            std::is_nothrow_move_constructible_v<TraitT> and std::is_nothrow_move_constructible_v<T>)
            : TraitT{ std::move(promote.getTrait()) }
            , counter{ new std::atomic_uint{ 1 } }
            , data{ promote.release() }
        {}

        explicit constexpr shared_resource(T value = TraitT::invalid_resource(), TraitT trait = {}) noexcept(
            std::is_nothrow_move_constructible_v<TraitT> and std::is_nothrow_move_constructible_v<T>)
            : TraitT{ std::move(trait) }
            , counter{ new std::atomic_uint{ 1 } }
            , data{ std::move(value) }
        {}

        shared_resource(const shared_resource &cpy)
            : counter{ cpy.counter }
            , data{ cpy.data }
        {
            ++(*counter);
        }

        shared_resource &operator=(const shared_resource &cpy)
        {
            data    = cpy.data;
            counter = cpy.counter;
            ++(*counter);
        }
        constexpr shared_resource(shared_resource &&move) noexcept
            : counter{ move.counter }
            , data{ std::move(move.data) }
        {
            move.data    = TraitT::invalid_resource();
            move.counter = nullptr;
        }

        shared_resource &operator=(shared_resource &&move) noexcept
        {
            if(this == &move)
                return *this;
            if constexpr(std::is_trivially_move_assignable_v<T>)
                data = move.data;
            else
                data = std::move(move.data);
            counter   = move.counter;
            move.data = TraitT::invalid_resource();
            return *this;
        }

        ~shared_resource() noexcept(std::is_nothrow_destructible_v<T> and std::is_nothrow_destructible_v<TraitT>
                                        and noexcept(std::declval<TraitT>().destroy(std::declval<T &>())))
        {
            if(data != TraitT::invalid_resource() && !--(*counter))
            {
                delete counter;
                TraitT::destroy(data);
            }
        }

        [[nodiscard]] bool isValid() const noexcept { return data != TraitT::invalid_resource(); }

        explicit operator bool() const noexcept { return isValid(); }

        [[nodiscard]] bool operator!() const { return data == TraitT::invalid_resource(); }

        [[nodiscard]] unsigned use_count() const noexcept { return *counter; }

        template<typename... ArgT>
        void create(ArgT... arg) noexcept(noexcept(std::declval<TraitT>().create(std::forward<ArgT>(arg)...)))
        {
            data    = TraitT::create(std::forward<ArgT>(arg)...);
            counter = new std::atomic_uint{ 1 };
        }

        void destroy() noexcept(noexcept(std::declval<TraitT>().destroy(std::declval<T &>())))
        {
            if(data != TraitT::invalid_resource() && !--(*counter))
            {
                delete counter;
                TraitT::destroy(data);
            }
            data    = TraitT::invalid_resource();
            counter = nullptr;
        }

        constexpr const T &getResource() const noexcept { return data; }

        TraitT &getTrait() noexcept { return *this; }

    private:
        std::atomic_uint *counter;
        T                 data;
    };
}// namespace core

#endif// BONGOMOUNTAIN_RESSOURCE_HPP
