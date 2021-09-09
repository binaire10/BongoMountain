#ifndef GAMEENGINEBONGO_RESSOURCE_HPP
#define GAMEENGINEBONGO_RESSOURCE_HPP

#include <utility>
#include <atomic>
#include <type_traits>

namespace Core
{
    template<typename T, typename TraitT>
    class resource : private TraitT
    {
    public:
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

        [[nodiscard]] bool isValid() const noexcept { return data != TraitT::invalid_resource(); }

        operator bool() const noexcept { return isValid(); }

        [[nodiscard]] bool operator!() const { return data == TraitT::invalid_resource(); }

        resource &operator=(resource &&move) noexcept
        {
            if(this == &move)
                return *this;
            if constexpr(std::is_trivially_move_assignable_v<T>)
                data = move.data;
            else
                data = std::move(move.data);
            move.data = TraitT::invalid_resource();
            return *this;
        }

        ~resource() noexcept(std::is_nothrow_destructible_v<T> and std::is_nothrow_destructible_v<TraitT> and noexcept(
            TraitT::destroy(std::declval<T>())))
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

    private:
        TraitT &getTrait() noexcept { return *this; }
        T       data;
    };

    template<typename T, typename TraitT>
    class shared_resource : private TraitT
    {
    public:
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
            move.data = TraitT::invalid_resource();
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
                                        and noexcept(TraitT::destroy(std::declval<T>())))
        {
            if(data != TraitT::invalid_resource() && !--(*counter))
            {
                delete counter;
                TraitT::destroy(data);
            }
        }

        [[nodiscard]] bool isValid() const noexcept { return data != TraitT::invalid_resource(); }

        operator bool() const noexcept { return isValid(); }

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
                data = TraitT::invalid_resource();
            }
        }

        constexpr const T &getResource() const noexcept { return data; }

    private:
        TraitT           &getTrait() noexcept { return *this; }
        std::atomic_uint *counter;
        T                 data;
    };
}// namespace Core

#endif// GAMEENGINEBONGO_RESSOURCE_HPP
