#ifndef WINDOWRESIZEEVENT_H
#define WINDOWRESIZEEVENT_H

#include <Graphic/config.hpp>
#include <cstdint>

#include <Event/Event.h>

class BM_GRAPHIC_DCL WindowResizeEvent : public Event
{
public:
    constexpr WindowResizeEvent(Graphic::RenderSurface *target, uint32_t width, uint32_t height)
        : target(target)
        , m_width{ width }
        , m_height{ height }
    {}

    [[nodiscard]] constexpr Graphic::RenderSurface *getTarget() const noexcept { return target; }

    [[nodiscard]] constexpr uint32_t getWidth() const { return m_width; }

    [[nodiscard]] constexpr uint32_t getHeight() const { return m_height; }

    EVENT_CLASS_CATEGORY(EventCategory::Application)
    EVENT_CLASS_TYPE(WindowResize)

private:
    Graphic::RenderSurface *target;
    uint32_t                m_width;
    uint32_t                m_height;
};


#endif// WINDOWRESIZEEVENT_H
