#ifndef KEYRELEASEDEVENT_H
#define KEYRELEASEDEVENT_H

#include "KeyEvent.h"

class BM_GRAPHIC_DCL KeyReleasedEvent : public KeyEvent
{
public:
    using KeyEvent::KeyEvent;
    EVENT_CLASS_TYPE(KeyReleased)
};


#endif// KEYRELEASEDEVENT_H
