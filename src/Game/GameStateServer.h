#pragma once

#include "GameState.h"

enum class SkillNum {
    First,  // Z
    Second, // X
    Third   // C
};


class GameStateServer : public GameState {
public:
    GameStateServer(Hero& firstHero_, Hero& secondHero_) = delete;
    explicit GameStateServer(Hero& firstHero_, Hero& secondHero_, double gameTick);

    void update();

    void applySkill(Player player, SkillNum skillNum);
    void applyMove(Player player, int32_t x, int32_t y);

    void applyDamage(double amount, Player player);
    void applyDamagePhys(double amount, Player player);
    void applyDamageMagic(double amount, Player player);

    void regenMana(double amount, Player player);
    void spendMana(double amount, Player player);
    [[nodiscard]] bool canSpendMana(double amount, Player player) const;

    [[nodiscard]] bool isDead(Player player) const;

private:
    double gameTick_;
};
