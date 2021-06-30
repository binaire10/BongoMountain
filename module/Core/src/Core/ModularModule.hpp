
#ifndef GAMEENGINEBONGO_MODULARMODULE_HPP
#define GAMEENGINEBONGO_MODULARMODULE_HPP

#include "IModule.hpp"

template<typename... R>
class StaticMod : private R...
{
public:
    void load()
    {
        call([](auto &g) { g.load(); }, *static_cast<R *>(this)...);
    }

    void unload()
    {
        call_inverted([](auto &g) { g.unload(); }, *static_cast<R *>(this)...);
    }

    void beforeFrame()
    {
        call([](auto &g) { g.beforeFrame(); }, *static_cast<R *>(this)...);
    }

    void afterFrame()
    {
        call_inverted([](auto &g) { g.afterFrame(); }, *static_cast<R *>(this)...);
    }

    template<class T>
    T &get() noexcept
    {
        return static_cast<T *>(this);
    }

    template<class T>
    const T &get() const noexcept
    {
        return static_cast<T *>(this);
    }

private:
    template<typename FunctionT, typename... ArgT>
    void call(FunctionT f, ArgT &...arg)
    {
        (f(arg), ...);
    }

    template<typename FunctionT>
    void call_inverted(FunctionT f)
    {}

    template<typename FunctionT, typename T, typename... ArgT>
    void call_inverted(FunctionT f, T &e, ArgT &...arg)
    {
        call_inverted(f, arg...);
        f(e);
    }
};

template<typename... R>
class ModularModule
    : public IModule
    , private StaticMod<R...>
{
public:
    void load() override { StaticMod<R...>::load(); }

    void unload() override { StaticMod<R...>::unload(); }

    void beforeFrame() override { StaticMod<R...>::beforeFrame(); }

    void afterFrame() override { StaticMod<R...>::afterFrame(); }

    using StaticMod<R...>::get;
};

#endif// GAMEENGINEBONGO_MODULARMODULE_HPP
