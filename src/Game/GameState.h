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

    void setHealthPoints(double amount, Player player);
    double getHealthPoints(Player player);

    bool gameIsFinished();

protected:
    Hero *firstHero;
    Hero *secondHero;
};


class GameStateServer : public GameState {
public:
    GameStateServer() = delete;
    explicit GameStateServer(double gameTick);

    void update();

    void damage(double amount, Player player);

private:
    double gameTick_;
};
