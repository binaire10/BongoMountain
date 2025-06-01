#ifndef KEYTYPEDEVENT_H
#define KEYTYPEDEVENT_H

#include "KeyEvent.h"

class BM_GRAPHIC_DCL KeyTypedEvent : public KeyEvent
{
public:
    using KeyEvent::KeyEvent;
    EVENT_CLASS_TYPE(KeyTyped)
};


#endif// KEYTYPEDEVENT_H
