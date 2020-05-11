#pragma once

#include <optional>
#include "Hero.h"
#include "Point.h"
#include "Creep.h"
#include "Events/Events.h"


enum class Player : uint8_t {
    First = 0,
    Second = 1
};

class GameState : public EventHandler<MoveEvent>,
                  public EventHandler<StopEvent>,
                  public EventHandler<FirstSkillUseEvent>,
                  public EventHandler<SecondSkillUseEvent>,
                  public EventHandler<ThirdSkillUseEvent> {
public:
    GameState();
    explicit GameState(Hero heroes_[NUM_PLAYERS], Creep creeps_[NUM_PLAYERS]);

    [[nodiscard]] bool gameIsFinished() const;

    [[nodiscard]] double getHealthPoints(Player player) const;
    [[nodiscard]] Point getPosition(Player player) const;
    [[nodiscard]] double getAngle(Player player) const;
    [[nodiscard]] Point getDestination(Player player) const;

    [[nodiscard]] bool isDead(Player player) const;

    void update(double elapsedTime); // time in milliseconds

    void refreshAllUnits();

    void setHealthPoints(double amount, Player player) const;
    void setPosition(Point pos, Player player) const;
    void setPosition(double x, double y, Player player) const;
    void setDestination(double x, double y, Player player) const;
    void setAngle(double angle, Player player) const;

    void applyDamage(double amount, Player player) const;
    void applyDamagePhys(double amount, Player player) const;
    void applyDamageMagic(double amount, Player player) const;

    void regenMana(double amount, Player player) const;
    void spendMana(double amount, Player player) const;
    [[nodiscard]] bool canSpendMana(double amount, Player player) const;

    [[nodiscard]] Hero *getHero(Player player) const;

    void serialize(BufferIO::BufferWriter &writer, Player player);
    void deserialize(BufferIO::BufferReader &reader, Player player);

    void handle(const MoveEvent &event) override;
    void handle(const StopEvent &event) override;
    void handle(const FirstSkillUseEvent &event) override;
    void handle(const SecondSkillUseEvent &event) override;
    void handle(const ThirdSkillUseEvent &event) override;

private:

    Hero *heroes_[NUM_PLAYERS];
    Creep *creeps_[NUM_PLAYERS];

    friend class Coil;
};
