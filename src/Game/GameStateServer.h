#pragma once

#include "GameState.h"

class GameStateServer : public GameState {
public:
    GameStateServer() = delete;
    explicit GameStateServer(double gameTick);

    void update();

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
