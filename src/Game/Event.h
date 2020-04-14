#pragma once

#include "GameState.h"

enum class Player;
class GameState;

enum class EventName {
    firstSkill,
    secondSkill,
    thirdSkill
};

class Event {
public:
    Event() = delete;
    inline explicit Event(Player player) : player_{player} {}

    virtual void handleEvent(GameState &gs) = 0;
protected:
    Player player_;
};
