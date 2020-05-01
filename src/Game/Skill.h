#pragma once

#include "Event.h"

enum class SkillNum : uint8_t {
    first = 0,  // Z
    second = 1, // X
    third = 2   // C
};

class GameState;

class Skill {
public:
    Skill(Player player, SkillNum type);

    void handleEvent(GameState &gs);
private:
    Player player_;
    SkillNum type_;
};
