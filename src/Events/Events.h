#pragma once

#include <vector>
#include "Utils/BufferIO.h"
#include "Game/Hero.h"
#include "Game/Attack.h"

enum class SerializedEventName : uint8_t {
    None = 0,
    FirstSkillUse = 1,
    SecondSkillUse = 2,
    ThirdSkillUse = 3,
    Move = 4,
    Stop = 5,
    Attack = 6
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
    virtual ~Event() = default;

    virtual void fire() = 0;
protected:
    template<typename T>
    static void fire(T *event);
};

class SerializedEvent : public Event {
public:
    static const SerializedEventName name_ = SerializedEventName::None;

    virtual void serialize(BufferIO::BufferWriter &writer) = 0;
};

class MoveEvent : public SerializedEvent {
public:
    static const SerializedEventName name_ = SerializedEventName::Move;

    Hero &hero_;
    double x_, y_;

    MoveEvent(Hero &hero, double x, double y);
    void serialize(BufferIO::BufferWriter &writer) override;
    void fire() override;
};

class StopEvent : public SerializedEvent {
public:
    static const SerializedEventName name_ = SerializedEventName::Stop;

    Hero &hero_;

    explicit StopEvent(Hero &hero);
    void serialize(BufferIO::BufferWriter &writer) override;
    void fire() override;
};

class AttackEvent : public SerializedEvent {
public:
    static const SerializedEventName name_ = SerializedEventName::Attack;

    uint8_t attackerID_;
    uint8_t targetID_;

    AttackEvent(uint8_t attackerID, uint8_t targetID);
    void serialize(BufferIO::BufferWriter &writer) override;
    void fire() override;
};

class SkillUseEvent : public SerializedEvent {
public:
    Hero &hero_;

    explicit SkillUseEvent(Hero &hero);
    void serialize(BufferIO::BufferWriter &writer) override = 0;
    void fire() override = 0;
};

class FirstSkillUseEvent : public SkillUseEvent {
public:
    static const SerializedEventName name_ = SerializedEventName::FirstSkillUse;

    explicit FirstSkillUseEvent(Hero &hero);
    void serialize(BufferIO::BufferWriter &writer) override;
    void fire() override;
};

class SecondSkillUseEvent : public SkillUseEvent {
public:
    static const SerializedEventName name_ = SerializedEventName::SecondSkillUse;

    explicit SecondSkillUseEvent(Hero &hero);
    void serialize(BufferIO::BufferWriter &writer) override;
    void fire() override;
};

class ThirdSkillUseEvent : public SkillUseEvent {
public:
    static const SerializedEventName name_ = SerializedEventName::ThirdSkillUse;

    explicit ThirdSkillUseEvent(Hero &hero);
    void serialize(BufferIO::BufferWriter &writer) override;
    void fire() override;
};

class GoldChangedEvent : public Event {
public:
    int gold_;
    void fire() override;
    GoldChangedEvent(int gold);
};

class DrawEvent : public Event {
public:
    void fire() override;
};

