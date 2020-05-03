#pragma once

#include <optional>
#include "StatsBuilder.h"
#include "Point.h"
#include "GameStateHandler.h"

class Hero;

enum class Player {
    First,
    Second
};

class GameState {
public:
    GameState();
    GameState(Hero &firstHero_, Hero &secondHero_);

    [[nodiscard]] bool gameIsFinished() const;

    [[nodiscard]] double getHealthPoints(Player player) const;
    [[nodiscard]] Point getPosition(Player player) const;
    [[nodiscard]] double getAngle(Player player) const;
    [[nodiscard]] Point getDestination(Player player) const;

    [[nodiscard]] bool isDead(Player player) const;

    void update(double elapsedTime); // time in milliseconds

    void setHealthPoints(double amount, Player player);
    void setPosition(Point pos, Player player);
    void setPosition(double x, double y, Player player);
    void setDestination(double x, double y, Player player);

    void applyDamage(double amount, Player player);
    void applyDamagePhys(double amount, Player player);
    void applyDamageMagic(double amount, Player player);

    void regenMana(double amount, Player player);
    void spendMana(double amount, Player player);
    [[nodiscard]] bool canSpendMana(double amount, Player player) const;

    [[nodiscard]] Hero *getHero(Player player) const;

private:

    Hero *firstHero_;
    Hero *secondHero_;

    friend GameStateHandler;
    GameStateHandler handler_;
};
