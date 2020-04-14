#pragma once

#include "Event.h"

enum class SkillNum {
    first,  // Z
    second, // X
    third   // C
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
