#include "Event/EventBus.h"

static EventBus* s_Instance = nullptr;

EventBus *EventBus::Get()
{
    if (s_Instance == nullptr) 
    {
        s_Instance = new EventBus();
    }

    return s_Instance;
}