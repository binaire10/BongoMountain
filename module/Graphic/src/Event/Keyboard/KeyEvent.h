#ifndef KEYEVENT_H
#define KEYEVENT_H

#include <Graphic/config.hpp>
#include <Event/Event.h>
#include <Core/KeyCodes.h>

class BM_GRAPHIC_DCL KeyEvent : public Event
{
public:
    constexpr explicit KeyEvent(Graphic::RenderSurface *target, KeyCode keyCode)
        : target{ target }
        , keyCode{ keyCode }
    {}

    [[nodiscard]] constexpr KeyCode getKeyCode() const noexcept { return keyCode; }

    [[nodiscard]] constexpr Graphic::RenderSurface *getTarget() const noexcept { return target; }

    EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input)

protected:
    Graphic::RenderSurface *target;
    KeyCode                 keyCode;
};


#endif// KEYEVENT_H
