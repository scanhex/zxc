#pragma once

#include "Hero.h"
#include "StatsBuilder.h"

enum class Player {
    First,
    Second
};


class GameState {
    // simple for now
public:
    GameState(Hero& firstHero_, Hero& secondHero_);
    ~GameState() = default;

    void setHealthPoints(double amount, Player player);
    double getHealthPoints(Player player);

    bool gameIsFinished();

protected:
    Hero &firstHero;
    Hero &secondHero;
};
