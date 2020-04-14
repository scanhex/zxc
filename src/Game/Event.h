#pragma once

#include <cstdint>
#include <cassert>

enum class Player;
class GameState;

enum class EventName {
    firstSkill,  // 1
    secondSkill, // 2
    thirdSkill   // 3
};

uint8_t eventNameToInt(EventName e);
EventName intToEventName(uint8_t eventId);


inline uint8_t eventNameToInt(EventName e){
    switch(e){
        case EventName::firstSkill:
            return 1;
        case EventName::secondSkill:
            return 2;
        case EventName::thirdSkill:
            return 3;
        default:
            assert(false);
    }
}

inline EventName intToEventName(uint8_t eventId){
    switch(eventId){
        case 1:
            return EventName::firstSkill;
        case 2:
            return EventName::secondSkill;
        case 3:
            return EventName::thirdSkill;
        default:
            assert(false);
    }
}

class Event {
public:
    Event() = delete;
    inline explicit Event(Player player) : player_{player} {}

    virtual void handleEvent(GameState &gs) = 0;
protected:
    Player player_;
};
