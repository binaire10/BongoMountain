#ifndef KEYEVENT_H
#define KEYEVENT_H

#include <graphic/config.hpp>
#include <event/Event.hpp>
#include <core/KeyCodes.h>
#include <render/RenderDevice.hpp>

class BM_GRAPHIC_DCL KeyEvent : public Event
{
public:
    constexpr explicit KeyEvent(render::RenderDevice *target, KeyCode keyCode)
        : target{ target }
        , keyCode{ keyCode }
    {}

    [[nodiscard]] constexpr KeyCode getKeyCode() const noexcept { return keyCode; }

    [[nodiscard]] constexpr render::RenderDevice *getTarget() const noexcept { return target; }

    EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input)

protected:
    render::RenderDevice *target;
    KeyCode                 keyCode;
};


#endif// KEYEVENT_H
