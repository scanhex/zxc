#pragma once

#include "Event.h"

enum class SkillNum {
    first,  // Z
    second, // X
    third   // C
};

class GameState;

class Skill : public Event {
public:
    Skill(Player player, SkillNum type);

    void handleEvent(GameState &gs) override;
private:
    SkillNum type_;
};
