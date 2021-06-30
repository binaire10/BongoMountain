#ifndef WINDOWCLOSEEVENT_H
#define WINDOWCLOSEEVENT_H

#include <Graphic/config.hpp>

#include <Event/Event.h>

class BM_GRAPHIC_DCL WindowCloseEvent : public Event
{
public:
    explicit constexpr WindowCloseEvent(Graphic::RenderSurface *target) noexcept
        : target(target)
    {}

    [[nodiscard]] constexpr Graphic::RenderSurface *getTarget() const noexcept { return target; }

    EVENT_CLASS_CATEGORY(EventCategory::Application)
    EVENT_CLASS_TYPE(WindowClose)

private:
    Graphic::RenderSurface *target;
};


#endif// WINDOWCLOSEEVENT_H
