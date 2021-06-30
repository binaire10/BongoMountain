#ifndef MOUSEBUTTONRELEASEDEVENT_H
#define MOUSEBUTTONRELEASEDEVENT_H

#include "MouseButtonEvent.h"

class BM_GRAPHIC_DCL MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    using MouseButtonEvent::MouseButtonEvent;
    EVENT_CLASS_TYPE(MouseButtonReleased)
};


#endif// MOUSEBUTTONRELEASEDEVENT_H
