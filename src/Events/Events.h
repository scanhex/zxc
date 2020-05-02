#pragma once

#include <vector>
#include "Utils/BufferIO.h"
#include "Game/Event.h"

#define DEFINE_HANDLER(handler_name) \
class handler_name##EventHandler { \
private: \
    static std::vector<handler_name##EventHandler*> allHandlers; \
public: \
static void fireEvent(handler_name##Event &event) { \
    for (auto &h : allHandlers) { \
        h->handle(event); \
    } \
} \
handler_name##EventHandler() { \
    allHandlers.push_back(this); \
} \
~handler_name##EventHandler() { \
    for (unsigned i = 0; i < allHandlers.size(); i++) { \
        if (allHandlers[i] == this) { \
            allHandlers[i] = allHandlers[allHandlers.size() - 1]; \
            if (!allHandlers.empty()) allHandlers.pop_back(); \
        } \
    } \
} \
virtual void handle(handler_name##Event &event) = 0; \
};

// TODO rename to Event
class NewEvent {
    virtual void serialize(uint8_t (&buffer)[1024]) = 0;
};

class MoveEvent : NewEvent {
public:
    static const EventName name = EventName::move;
    double x, y;

    void serialize(uint8_t (&buffer)[1024]) override {
        BufferIO::writeUInt8(static_cast<uint8_t>(name), 0, buffer);
        BufferIO::writeDouble(x, 1, buffer);
        BufferIO::writeDouble(y, 9, buffer);
    }
};

DEFINE_HANDLER(Move)
