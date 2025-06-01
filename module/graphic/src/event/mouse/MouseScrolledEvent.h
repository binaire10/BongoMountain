#ifndef MOUSESCROLLEDEVENT_H
#define MOUSESCROLLEDEVENT_H

#include <Graphic/config.hpp>
#include <Event/Event.hpp>
#include <render/RenderDevice.hpp>

class BM_GRAPHIC_DCL MouseScrolledEvent : public Event
{
public:
    constexpr MouseScrolledEvent(render::RenderDevice *target, float xOffset, float yOffset)
        : target{ target }
        , m_xOffset(xOffset)
        , m_yOffset(yOffset)
    {}

    [[nodiscard]] constexpr float getXOffset() const noexcept { return m_xOffset; }

    [[nodiscard]] constexpr float getYOffset() const noexcept { return m_yOffset; }

    [[nodiscard]] constexpr render::RenderDevice *getTarget() const noexcept { return target; }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)

private:
    render::RenderDevice *target;
    float                   m_xOffset;
    float                   m_yOffset;
};


#endif// MOUSESCROLLEDEVENT_H
