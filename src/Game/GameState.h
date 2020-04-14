#pragma once

enum class Player {
    First,
    Second
};

#include <optional>
#include "Hero.h"
#include "StatsBuilder.h"

class Hero;
enum class EventName;


class GameState {
public:
    GameState();
    GameState(Hero &firstHero_, Hero &secondHero_);

    bool gameIsFinished();

    double getHealthPoints(Player player);

    void update(double elapsedTime);

    [[nodiscard]] bool isDead(Player player) const;

    void applyMove(Player player, int32_t x, int32_t y);
    void applyEvent(Player player, EventName eventName);

    void setHealthPoints(double amount, Player player);

    void applyDamage(double amount, Player player);
    void applyDamagePhys(double amount, Player player);
    void applyDamageMagic(double amount, Player player);

    void regenMana(double amount, Player player);
    void spendMana(double amount, Player player);
    [[nodiscard]] bool canSpendMana(double amount, Player player) const;

private:
    Hero *firstHero_;
    Hero *secondHero_;
};
