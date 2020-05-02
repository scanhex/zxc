#pragma once

#include <cstdint>
#include <cassert>
#include <optional>

enum class Player;
class GameState;

enum class EventName : uint8_t {
    None = 0,
    ShortCoilUse = 1,
    MidCoilUse = 2,
    LongCoilUse = 3,
    Move = 4
};

class Event {
public:
    Event();
    explicit Event(EventName eventName, Player player);
    Event(EventName eventName, Player player, double x, double y);

    EventName eventName_;
    Player player_;
    double x_, y_;
};
