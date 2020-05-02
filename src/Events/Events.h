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
    virtual void serialize(BufferIO::BufferWriter& writer) = 0;
};

class MoveEvent : NewEvent {
public:
    static const EventName name = EventName::move;
    double x_, y_;

    MoveEvent(double x, double y) : x_(x), y_(y) {}

    void serialize(BufferIO::BufferWriter& writer) override {
        writer.writeUInt8(static_cast<uint8_t>(name));
        writer.writeDouble(x_);
        writer.writeDouble(y_);
    }
};