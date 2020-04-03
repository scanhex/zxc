#include "GameState.h"

GameState::GameState() {
    StatsBuilder heroStatsBuilder = StatsBuilder().
            setDamage(100).
            setAttackRange(100).
            setMoveSpeed(350).
            setAttackSpeed(100).
            setMaxHp(1000).
            setMaxMp(300).
            setHpRegen(2).
            setMpRegen(1).
            setArmor(3).
            setResist(0.25);
    firstHero = new Hero(heroStatsBuilder.create(), Point(0, 0));
    secondHero = new Hero(heroStatsBuilder.create(), Point(100, 100));
}

double GameState::getHealthPoints(Player player) {
    switch (player) {
        case Player::First:
            return firstHero->getHealthPoints();
        case Player::Second:
            return secondHero->getHealthPoints();
        default:
            assert(false);
    }
}

void GameState::setHealthPoints(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero->setHealthPoints(amount);
        case Player::Second:
            secondHero->setHealthPoints(amount);
        default:
            assert(false);
    }
}


GameStateServer::GameStateServer(double gameTick) : GameState(), gameTick_{gameTick} {}

void GameStateServer::update() {
    // only hp regen for now
    double healPerTick = firstHero->getHpRegen() * gameTick_;
    firstHero->heal(healPerTick);

    healPerTick = secondHero->getHpRegen() * gameTick_;
    secondHero->heal(healPerTick);
}

void GameStateServer::damage(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero->damage(amount);
            break;
        case Player::Second:
            secondHero->damage(amount);
            break;
        default:
            assert(false);
    }
}
