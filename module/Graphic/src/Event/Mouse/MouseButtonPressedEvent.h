#ifndef MOUSEBUTTONPRESSEDEVENT_H
#define MOUSEBUTTONPRESSEDEVENT_H

#include "MouseButtonEvent.h"

class BM_GRAPHIC_DCL MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    using MouseButtonEvent::MouseButtonEvent;
    EVENT_CLASS_TYPE(MouseButtonPressed)
};


#endif// MOUSEBUTTONPRESSEDEVENT_H
