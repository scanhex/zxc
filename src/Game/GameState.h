#pragma once

#include "Hero.h"
#include "StatsBuilder.h"

enum class Player{
    First,
    Second
};


class GameState{
    // simple for now
public:
    GameState() = delete;
    explicit GameState(double gameTick);

    void update();

    void damage(double amount, Player player);

    double getHealthPoints(Player player);

private:
    double gameTick_;
    Hero* firstHero;
    Hero* secondHero;
};
