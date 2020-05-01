#pragma once

#include <cstdint>
#include <cassert>
#include <optional>

enum class Player;
class GameState;

enum class EventName : uint8_t {
    firstSkill  = 1,  // 1
    secondSkill = 2, // 2
    thirdSkill  = 3,  // 3
    move        = 4  // 4 (+ x & y)
};

class Event {
public:
    Event();
    explicit Event(EventName eventName, Player player);
    Event(EventName eventName, Player player, double x, double y);
    static EventName intToEventName(uint8_t eventId);
    uint8_t eventNameToInt();

    EventName eventName_;
    Player player_;
    double x_, y_;
};
