#ifndef MOUSEMOVEDEVENT_H
#define MOUSEMOVEDEVENT_H

#include <Graphic/config.hpp>
#include <Event/Event.hpp>
#include <render/RenderDevice.hpp>

class BM_GRAPHIC_DCL MouseMovedEvent : public Event
{
public:
    constexpr MouseMovedEvent(render::RenderDevice *target, float mouseX, float mouseY)
        : target{ target }
        , m_mouseX(mouseX)
        , m_mouseY(mouseY)
    {}

    [[nodiscard]] constexpr render::RenderDevice *getTarget() const noexcept { return target; }

    [[nodiscard]] constexpr float getMouseX() const noexcept { return m_mouseX; }

    [[nodiscard]] constexpr float getMouseY() const noexcept { return m_mouseY; }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)

private:
    render::RenderDevice *target;
    float                   m_mouseX;
    float                   m_mouseY;
};


#endif// MOUSEMOVEDEVENT_H
