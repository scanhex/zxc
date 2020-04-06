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
    GameState();
    ~GameState();

    void setHealthPoints(double amount, Player player);
    double getHealthPoints(Player player);

    bool gameIsFinished();

protected:
    Hero *firstHero;
    Hero *secondHero;
};
