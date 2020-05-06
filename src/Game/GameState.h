#pragma once

#include <optional>
#include "StatsBuilder.h"
#include "Point.h"
#include "Events/Events.h"

class Hero;

enum class Player {
    First,
    Second
};

class GameState : public EventHandler<MoveEvent>,
                  public EventHandler<StopEvent>,
                  public EventHandler<FirstSkillUseEvent>,
                  public EventHandler<SecondSkillUseEvent>,
                  public EventHandler<ThirdSkillUseEvent> {
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

    void handle(const MoveEvent &event) override;
    void handle(const StopEvent &event) override;
    void handle(const FirstSkillUseEvent &event) override;
    void handle(const SecondSkillUseEvent &event) override;
    void handle(const ThirdSkillUseEvent &event) override;

private:

    Hero *firstHero_;
    Hero *secondHero_;
};
