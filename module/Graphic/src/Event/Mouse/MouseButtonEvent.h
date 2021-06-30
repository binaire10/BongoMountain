#ifndef MOUSEBUTTONEVENT_H
#define MOUSEBUTTONEVENT_H

#include <Graphic/config.hpp>
#include <Core/MouseCode.h>
#include <Event/Event.h>

class BM_GRAPHIC_DCL MouseButtonEvent : public Event
{
public:
    explicit constexpr MouseButtonEvent(Graphic::RenderSurface *target, MouseCode button)
        : target{ target }
        , button(button)
    {}

    [[nodiscard]] constexpr Graphic::RenderSurface *getTarget() const noexcept { return target; }

    [[nodiscard]] constexpr MouseCode getButton() const noexcept { return button; }

    EVENT_CLASS_CATEGORY(EventCategory::MouseButton | EventCategory::Input)
private:
    Graphic::RenderSurface *target;
    MouseCode               button;
};


#endif// MOUSEBUTTONEVENT_H
