#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include "Event.h"

class EventDispatcher
{
public:
    constexpr explicit EventDispatcher(Event &e)
        : m_event{ e }
    {}

    template<typename T, typename F>
    bool dispatch(const F &func)
    {
        if(m_event.getType() == T::getStaticType())
        {
            if constexpr(std::is_same_v<decltype(func(static_cast<T &>(m_event))), void>)
                func(static_cast<T &>(m_event));
            else
                return func(static_cast<T &>(m_event));
            return true;
        }
        return false;
    }

private:
    Event &m_event;
};

#endif// EVENTDISPATCHER_H
