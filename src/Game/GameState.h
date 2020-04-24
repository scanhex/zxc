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

    [[nodiscard]] bool gameIsFinished() const;

    [[nodiscard]] double getHealthPoints(Player player) const;
    [[nodiscard]] Point getPosition(Player player) const;
    [[nodiscard]] double getAngle(Player player) const;

    [[nodiscard]] bool isDead(Player player) const;

    void update(double elapsedTime); // time in milliseconds

    void applyMove(Player player, double x, double y);
    void applyEvent(Event event);

    void setHealthPoints(double amount, Player player);
    void setPosition(Point pos, Player player);
    void setPosition(double x, double y, Player player);

    void applyDamage(double amount, Player player);
    void applyDamagePhys(double amount, Player player);
    void applyDamageMagic(double amount, Player player);

    void regenMana(double amount, Player player);
    void spendMana(double amount, Player player);
    [[nodiscard]] bool canSpendMana(double amount, Player player) const;

private:
    [[nodiscard]] Hero *getHero(Player player) const;

    Hero *firstHero_;
    Hero *secondHero_;
};
