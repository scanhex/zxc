#pragma once

#include "GameState.h"

enum class EventName{
    firstSkill,
    secondSkill,
    thirdSkill
};

class Event {
public:
    Event() = delete;
    explicit Event(Player player);

    virtual void handleEvent(GameState &gs) = 0;
protected:
    Player player_;
};

Event::Event(Player player) : player_{player} {}
