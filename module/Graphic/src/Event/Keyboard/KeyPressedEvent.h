#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include "KeyEvent.h"

class BM_GRAPHIC_DCL KeyPressedEvent : public KeyEvent
{
public:
    constexpr KeyPressedEvent(Graphic::RenderSurface *target, KeyCode keyCode, unsigned repeat)
        : KeyEvent(target, keyCode)
        , m_repeat(repeat)
    {}

    [[nodiscard]] constexpr unsigned getRepeat() const noexcept { return m_repeat; }

    EVENT_CLASS_TYPE(KeyPressed)

protected:
    unsigned m_repeat;
};


#endif// KEYPRESSEDEVENT_H
