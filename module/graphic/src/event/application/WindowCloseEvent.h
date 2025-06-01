#ifndef WINDOWCLOSEEVENT_H
#define WINDOWCLOSEEVENT_H

#include <graphic/config.hpp>
#include <render/RenderDevice.hpp>
#include <event/Event.hpp>

class BM_GRAPHIC_DCL WindowCloseEvent : public Event
{
public:
    explicit constexpr WindowCloseEvent(render::RenderDevice *target) noexcept
        : target(target)
    {}

    [[nodiscard]] constexpr render::RenderDevice *getTarget() const noexcept { return target; }

    EVENT_CLASS_CATEGORY(EventCategory::Application)
    EVENT_CLASS_TYPE(WindowClose)

private:
    render::RenderDevice *target;
};


#endif// WINDOWCLOSEEVENT_H
