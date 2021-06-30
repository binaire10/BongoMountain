#ifndef GAMEENGINEBONGO_RESSOURCE_HPP
#define GAMEENGINEBONGO_RESSOURCE_HPP

#include <utility>
#include <type_traits>

template<typename T, typename TraitT>
class Resource : private TraitT
{
public:
    explicit constexpr Resource(T value = TraitT::invalid_resource(), TraitT trait = {}) noexcept(
        std::is_nothrow_move_constructible_v<TraitT> and std::is_nothrow_move_constructible_v<T>)
        : TraitT{ std::move(trait) }
        , resource{ std::move(value) }
    {}
    Resource(const Resource &) = delete;
    Resource &operator=(const Resource &) = delete;
    constexpr Resource(Resource &&move) noexcept
        : resource{ std::move(move.resource) }
    {
        move.resource = TraitT::invalid_resource();
    }

    Resource &operator=(Resource &&move) noexcept
    {
        if(this == &move)
            return *this;
        if constexpr(std::is_trivially_move_assignable_v<T>)
            resource = move.resource;
        else
            resource = std::move(move.resource);
        move.resource = TraitT::invalid_resource();
        return *this;
    }

    ~Resource() noexcept(std::is_nothrow_destructible_v<T> and std::is_nothrow_destructible_v<TraitT> and noexcept(
        TraitT::destroy(std::declval<T>())))
    {
        if(resource != TraitT::invalid_resource())
            TraitT::destroy(resource);
    }

    template<typename... ArgT>
    void create(ArgT... arg) noexcept(noexcept(std::declval<TraitT>().create(std::forward<ArgT>(arg)...)))
    {
        resource = TraitT::create(std::forward<ArgT>(arg)...);
    }

    void destroy() noexcept(noexcept(std::declval<TraitT>().destroy(std::declval<T &>())))
    {
        if(resource != TraitT::invalid_resource())
        {
            TraitT::destroy(resource);
            resource = TraitT::invalid_resource();
        }
    }

    constexpr const T &getResource() const noexcept { return resource; }

    T release() noexcept
    {
        T tmp    = std::move(resource);
        resource = TraitT::invalid_resource();
        return tmp;
    }

private:
    TraitT &getTrait() noexcept { return *this; }
    T       resource;
};

#endif// GAMEENGINEBONGO_RESSOURCE_HPP
