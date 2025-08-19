#include "miko/utils/Event.h"

namespace miko {

// EventDispatcher implementation
void EventDispatcher::Clear() {
    handlers.clear();
}

// Global event dispatcher instance
static EventDispatcher g_eventDispatcher;

EventDispatcher& GetEventDispatcher() {
    return g_eventDispatcher;
}

} // namespace miko