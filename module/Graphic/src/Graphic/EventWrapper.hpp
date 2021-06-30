#ifndef GAMEENGINEBONGO_EVENTWRAPER_HPP
#define GAMEENGINEBONGO_EVENTWRAPER_HPP

#include "RenderSurface.hpp"

namespace Graphic
{
    template<typename EventT>
    class EventWrapper : public EventT
    {
    public:
        template<typename... ArgT>
        explicit constexpr EventWrapper(RenderSurface *target, ArgT... arg) noexcept
            : EventT{ std::forward<ArgT>(arg)... }
            , target{ target }
        {}

        [[nodiscard]] RenderSurface *getTarget() const noexcept { return target; }

    private:
        RenderSurface *target{};
    };
}// namespace Graphic
#endif// GAMEENGINEBONGO_EVENTWRAPER_HPP
