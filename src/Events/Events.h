#pragma once

#include <vector>
#include "Game/Hero.h"
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
    for (std::size_t i = 0; i < allHandlers.size(); i++) { \
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
public:
    static const EventName name_ = EventName::None;

    virtual void serialize(uint8_t (&buffer)[1024]) = 0;
};

class MoveEvent : NewEvent {
public:
    static const EventName name_ = EventName::Move;

    Hero &hero_;
    double x_, y_;

    MoveEvent(Hero &hero, double x, double y);
    void serialize(uint8_t (&buffer)[1024]) override;
};

class SkillUseEvent : NewEvent {
public:
    explicit SkillUseEvent(Hero &hero);
    void serialize(uint8_t (&buffer)[1024]) override;
    Hero &hero_;
};

class ShortCoilUseEvent : SkillUseEvent {
public:
    static const EventName name_ = EventName::ShortCoilUse;
};

class MidCoilUseEvent : SkillUseEvent {
public:
    static const EventName name_ = EventName::MidCoilUse;
};

class LongCoilUseEvent : SkillUseEvent {
public:
    static const EventName name_ = EventName::LongCoilUse;
};

DEFINE_HANDLER(Move)

DEFINE_HANDLER(SkillUse)

class ShortCoilUseEventHandler : SkillUseEventHandler {
    void handle(SkillUseEvent &event) override = 0;
};

class MidCoilUseEventHandler : SkillUseEventHandler {
    void handle(SkillUseEvent &event) override = 0;
};

class LongCoilUseEventHandler : SkillUseEventHandler {
    void handle(SkillUseEvent &event) override = 0;
};

