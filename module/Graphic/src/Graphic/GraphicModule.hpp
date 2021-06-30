#ifndef GAMEENGINEBONGO_GRAPHICMODULE_HPP
#define GAMEENGINEBONGO_GRAPHICMODULE_HPP

#include <Core/IModule.hpp>
#include "Context.hpp"

namespace Graphic
{
    template<typename... R>
    class StaticMod : private R...
    {
    public:
        template<typename... ArgT>
        void load(ArgT... arg)
        {
            call([&](auto &g) { g.load(std::forward<ArgT>(arg)...); }, *static_cast<R *>(this)...);
        }

        template<typename... ArgT>
        void unload(ArgT... arg)
        {
            call_inverted([&](auto &g) { g.unload(std::forward<ArgT>(arg)...); }, *static_cast<R *>(this)...);
        }

        template<typename... ArgT>
        void beforeFrame(ArgT... arg)
        {
            call([&](auto &g) { g.beforeFrame(std::forward<ArgT>(arg)...); }, *static_cast<R *>(this)...);
        }

        template<typename... ArgT>
        void afterFrame(ArgT... arg)
        {
            call_inverted([&](auto &g) { g.afterFrame(std::forward<ArgT>(arg)...); }, *static_cast<R *>(this)...);
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
        static void call(FunctionT f, ArgT &...arg)
        {
            (f(arg), ...);
        }

        template<typename FunctionT>
        static void call_inverted(FunctionT f) noexcept
        {}

        template<typename FunctionT, typename T, typename... ArgT>
        static void call_inverted(FunctionT f, T &e, ArgT &...arg)
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
        void load() override { StaticMod<R...>::load(std::ref(context)); }

        void unload() override { StaticMod<R...>::unload(std::ref(context)); }

        void beforeFrame() override { StaticMod<R...>::beforeFrame(std::ref(context)); }

        void afterFrame() override { StaticMod<R...>::afterFrame(std::ref(context)); }

        using StaticMod<R...>::get;

        [[nodiscard]] const GlobalContext &getContext() const { return context; }

        [[nodiscard]] GlobalContext &getContext() { return context; }

    private:
        GlobalContext context;
    };
}// namespace Graphic

#endif// GAMEENGINEBONGO_GRAPHICMODULE_HPP
