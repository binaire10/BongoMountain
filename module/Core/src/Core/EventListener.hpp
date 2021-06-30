#ifndef GAMEENGINEBONGO_EVENTLISTENER_HPP
#define GAMEENGINEBONGO_EVENTLISTENER_HPP

#include <Event/Event.h>

class EventListener
{
public:
    virtual ~EventListener()      = default;
    virtual void handle(Event &e) = 0;
};

#endif// GAMEENGINEBONGO_EVENTLISTENER_HPP
