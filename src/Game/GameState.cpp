#include "GameState.h"
#include <algorithm>

GameState::GameState(Hero &firstHero, Hero &secondHero) : firstHero_{&firstHero},
                                                          secondHero_{&secondHero},
                                                          handler_{*this} {}

GameState::GameState() : handler_{*this} {
    // 2 default heros
    firstHero_ = new Hero(Player::First);
    secondHero_ = new Hero(Player::Second);
}

Hero *GameState::getHero(Player player) const {
    switch (player) {
        case Player::First:
            return firstHero_;
        case Player::Second:
            return secondHero_;
        default:
            assert(false);
    }
}

double GameState::getHealthPoints(Player player) const {
    Hero *hero = getHero(player);
    return hero->getHealthPoints();
}

Point GameState::getPosition(Player player) const {
    Hero *hero = getHero(player);
    return hero->getPosition();
}

double GameState::getAngle(Player player) const {
    Hero *hero = getHero(player);
    return hero->getAngle();
}

Point GameState::getDestination(Player player) const {
    Hero *hero = getHero(player);
    return hero->getDestination();
}

void GameState::setPosition(Point pos, Player player) {
    Hero *hero = getHero(player);
    hero->setPosition(pos);
}

void GameState::setPosition(double x, double y, Player player) {
    Hero *hero = getHero(player);
    hero->setPosition(x, y);
}

void GameState::setHealthPoints(double amount, Player player) {
    Hero *hero = getHero(player);
    hero->setHealthPoints(amount);
}

void GameState::setDestination(double x, double y, Player player) {
    Hero *hero = getHero(player);
    hero->setDestination(x, y);
}

bool GameState::gameIsFinished() const {
    return firstHero_->isDead() || secondHero_->isDead();
}

void GameState::update(double elapsedTime) { // time in milliseconds
    assert(elapsedTime >= 0);
    double elapsedTimeInSeconds = elapsedTime / 1000.0;

    for (Hero *hero : {firstHero_, secondHero_}) {
        if (!hero->isDead()) {
            hero->updateUnit(elapsedTimeInSeconds);
        }
    }
}

void GameState::applyDamage(double amount, Player player) {
    Hero *hero = getHero(player);
    hero->applyDamage(amount);
}

void GameState::applyDamagePhys(double amount, Player player) {
    Hero *hero = getHero(player);
    hero->applyDamagePhys(amount);
}

void GameState::applyDamageMagic(double amount, Player player) {
    Hero *hero = getHero(player);
    hero->applyDamageMagic(amount);
}

void GameState::regenMana(double amount, Player player) {
    Hero *hero = getHero(player);
    hero->regenMana(amount);
}

void GameState::spendMana(double amount, Player player) {
    Hero *hero = getHero(player);
    hero->spendMana(amount);
}

bool GameState::canSpendMana(double amount, Player player) const {
    Hero *hero = getHero(player);
    return hero->canSpendMana(amount);
}

bool GameState::isDead(Player player) const {
    Hero *hero = getHero(player);
    return hero->isDead();
}
