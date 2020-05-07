#pragma once

#include <vector>
#include "Utils/BufferIO.h"
#include "Game/Hero.h"

enum class EventName : uint8_t {
    None = 0,
    FirstSkillUse = 1,
    SecondSkillUse = 2,
    ThirdSkillUse = 3,
    Move = 4,
    Stop = 5
};

template<class Event>
class EventHandler {
private:
    static std::vector<EventHandler<Event> *> allHandlers;
public:
    static void fireEvent(const Event &event) {
        for (auto &h : allHandlers) {
            h->handle(event);
        }
    }

    EventHandler() {
        allHandlers.push_back(this);
    }

    ~EventHandler() {
        for (size_t i = 0; i < allHandlers.size(); i++) {
            if (allHandlers[i] == this) {
                allHandlers[i] = allHandlers[allHandlers.size() - 1];
                if (!allHandlers.empty()) allHandlers.pop_back();
            }
        }
    }

    virtual void handle(const Event &event) = 0;
};

template<class Event>
std::vector<EventHandler<Event> *> EventHandler<Event>::allHandlers{};

class Event {
public:
    static const EventName name_ = EventName::None;

    Event();
    virtual ~Event() = default;

    virtual void serialize(BufferIO::BufferWriter &writer) = 0;
    virtual void fire() = 0;

    bool need_send_;
};

class MoveEvent : public Event {
public:
    static const EventName name_ = EventName::Move;

    Hero &hero_;
    double x_, y_;

    MoveEvent(Hero &hero, double x, double y);
    void serialize(BufferIO::BufferWriter &writer) override;
    void fire() override;
};

class StopEvent : public Event {
public:
    static const EventName name_ = EventName::Stop;

    Hero &hero_;

    explicit StopEvent(Hero &hero);
    void serialize(BufferIO::BufferWriter &writer) override;
    void fire() override;
};

class SkillUseEvent : public Event {
public:
    Hero &hero_;

    explicit SkillUseEvent(Hero &hero);
    void serialize(BufferIO::BufferWriter &writer) override = 0;
    void fire() override = 0;
};

class FirstSkillUseEvent : public SkillUseEvent {
public:
    static const EventName name_ = EventName::FirstSkillUse;

    explicit FirstSkillUseEvent(Hero &hero);
    void serialize(BufferIO::BufferWriter &writer) override;
    void fire() override;
};

class SecondSkillUseEvent : public SkillUseEvent {
public:
    static const EventName name_ = EventName::SecondSkillUse;

    explicit SecondSkillUseEvent(Hero &hero);
    void serialize(BufferIO::BufferWriter &writer) override;
    void fire() override;
};

class ThirdSkillUseEvent : public SkillUseEvent {
public:
    static const EventName name_ = EventName::ThirdSkillUse;

    explicit ThirdSkillUseEvent(Hero &hero);
    void serialize(BufferIO::BufferWriter &writer) override;
    void fire() override;
};
