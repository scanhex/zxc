#pragma once

#include <vector>
#include "Utils/BufferIO.h"
#include "Game/Event.h"

template <class Event>
class EventHandler {
private:
    static std::vector<EventHandler<Event>*> allHandlers;
public:
    static void fireEvent(Event &event) {
        for (auto &h : allHandlers) {
            h->handle(event);
        }
    }

    EventHandler() {
        allHandlers.push_back(this);
    }

    ~EventHandler() {
        for (unsigned i = 0; i < allHandlers.size(); i++) {
            if (allHandlers[i] == this) {
                allHandlers[i] = allHandlers[allHandlers.size() - 1];
                if (!allHandlers.empty()) allHandlers.pop_back();
            }
        }
    }

    virtual void handle(Event &event) = 0;
};

// TODO rename to Event
class NewEvent {
    virtual void serialize(uint8_t (&buffer)[1024]) = 0;
};

class MoveEvent : NewEvent {
public:
    static const EventName name = EventName::move;
    double x_, y_;

    MoveEvent(double x, double y) : x_(x), y_(y) {}

    void serialize(uint8_t (&buffer)[1024]) override {
        BufferIO::writeUInt8(static_cast<uint8_t>(name), 0, buffer);
        BufferIO::writeDouble(x_, 1, buffer);
        BufferIO::writeDouble(y_, 9, buffer);
    }
};