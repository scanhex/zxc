#include "GameState.h"
#include "GameStateServer.h"

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

GameState::~GameState() {
    delete firstHero;
    delete secondHero;
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
            break;
        case Player::Second:
            secondHero->setHealthPoints(amount);
            break;
        default:
            assert(false);
    }
}

bool GameState::gameIsFinished() {
    return firstHero->isDead() || secondHero->isDead();
}


GameStateServer::GameStateServer(double gameTick) : GameState(), gameTick_{gameTick} {}

void GameStateServer::update() {
    // only regen for now
    double healPerTick = firstHero->getHpRegen() * gameTick_;
    double manaPerTick = firstHero->getMpRegen() * gameTick_;
    firstHero->applyHeal(healPerTick);
    firstHero->regenMana(manaPerTick);

    healPerTick = secondHero->getHpRegen() * gameTick_;
    manaPerTick = secondHero->getMpRegen() * gameTick_;
    secondHero->applyHeal(healPerTick);
    secondHero->regenMana(manaPerTick);
}

void GameStateServer::applyDamage(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero->applyDamage(amount);
            break;
        case Player::Second:
            secondHero->applyDamage(amount);
            break;
        default:
            assert(false);
    }
}

void GameStateServer::applyDamagePhys(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero->applyDamagePhys(amount);
            break;
        case Player::Second:
            secondHero->applyDamagePhys(amount);
            break;
        default:
            assert(false);
    }
}

void GameStateServer::applyDamageMagic(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero->applyDamageMagic(amount);
            break;
        case Player::Second:
            secondHero->applyDamageMagic(amount);
            break;
        default:
            assert(false);
    }
}

void GameStateServer::regenMana(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero->regenMana(amount);
            break;
        case Player::Second:
            secondHero->regenMana(amount);
            break;
        default:
            assert(false);
    }
}

void GameStateServer::spendMana(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero->spendMana(amount);
            break;
        case Player::Second:
            secondHero->spendMana(amount);
            break;
        default:
            assert(false);
    }
}

bool GameStateServer::canSpendMana(double amount, Player player) const {
    switch (player) {
        case Player::First:
            return firstHero->canSpendMana(amount);
        case Player::Second:
            return secondHero->canSpendMana(amount);
        default:
            assert(false);
    }
}

bool GameStateServer::isDead(Player player) const {
    switch (player) {
        case Player::First:
            return firstHero->isDead();
        case Player::Second:
            return secondHero->isDead();
        default:
            assert(false);
    }
}
