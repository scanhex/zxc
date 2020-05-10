#define _USE_MATH_DEFINES

#include "GameState.h"
#include <cmath>
#include <algorithm>

GameState::GameState(Hero &firstHero, Hero &secondHero) : firstHero_{&firstHero},
                                                          secondHero_{&secondHero} {}

GameState::GameState() {
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

void GameState::setPosition(Point pos, Player player) const {
    Hero *hero = getHero(player);
    hero->setPosition(pos);
}

void GameState::setPosition(double x, double y, Player player) const {
    Hero *hero = getHero(player);
    hero->setPosition(x, y);
}

void GameState::setHealthPoints(double amount, Player player) const {
    Hero *hero = getHero(player);
    hero->setHealthPoints(amount);
}

void GameState::setDestination(double x, double y, Player player) const {
    Hero *hero = getHero(player);
    hero->setDestination(x, y);
}

void GameState::setAngle(double angle, Player player) const {
    Hero *hero = getHero(player);
    hero->setAngle(angle);
}

bool GameState::gameIsFinished() const {
    return firstHero_->getDeathCounter() >= 2 || secondHero_->getDeathCounter() >= 2;
}

void GameState::update(double elapsedTime) { // time in milliseconds
    assert(elapsedTime >= 0);
    double elapsedTimeInSeconds = elapsedTime / 1000.0;

    for (Hero *hero : {firstHero_, secondHero_}) {
        if (!hero->isDead()) {
            hero->updateUnit(elapsedTimeInSeconds);
        } else {
            if (hero->getDeathCounter() < 1) {
                hero->increaseDeathCounter();
                hero->refreshUnit();
            }
        }
    }
}

void GameState::refreshAllUnits() {
    for (Hero *hero : {firstHero_, secondHero_}) {
        hero->setDeathCounter(0);
        hero->refreshUnit();
    }
}


void GameState::applyDamage(double amount, Player player) const {
    Hero *hero = getHero(player);
    hero->applyDamage(amount);
}

void GameState::applyDamagePhys(double amount, Player player) const {
    Hero *hero = getHero(player);
    hero->applyDamagePhys(amount);
}

void GameState::applyDamageMagic(double amount, Player player) const {
    Hero *hero = getHero(player);
    hero->applyDamageMagic(amount);
}

void GameState::regenMana(double amount, Player player) const {
    Hero *hero = getHero(player);
    hero->regenMana(amount);
}

void GameState::spendMana(double amount, Player player) const {
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

void GameState::serialize(BufferIO::BufferWriter &writer, Player player) {
    Team team = static_cast<Team>(player);
    // first serialize player
    for (Hero *hero : {firstHero_, secondHero_}) {
        if (hero->getTeam() == team) {
            hero->serialize(writer);
        }
    }
    // then every other unit
    for (Hero *hero : {firstHero_, secondHero_}) {
        if (hero->getTeam() != team) {
            hero->serialize(writer);
        }
    }
}

void GameState::deserialize(BufferIO::BufferReader &reader, Player player) {
    Team team = static_cast<Team>(player);
    // first deserialize player
    for (Hero *hero : {firstHero_, secondHero_}) {
        if (hero->getTeam() == team) {
            hero->deserialize(reader);
        }
    }
    // then every other unit
    for (Hero *hero : {firstHero_, secondHero_}) {
        if (hero->getTeam() != team) {
            hero->deserialize(reader);
        }
    }
}

void GameState::handle(const MoveEvent &event) {
    event.hero_.setDestination(event.x_, event.y_);
}

void GameState::handle(const StopEvent &event) {
    Point curPosition = event.hero_.getPosition();
    event.hero_.setDestination(curPosition);
}

void GameState::handle(const FirstSkillUseEvent &event) {
    event.hero_.useSkill(SkillName::FirstSkill, *this);
}

void GameState::handle(const SecondSkillUseEvent &event) {
    event.hero_.useSkill(SkillName::SecondSkill, *this);
}

void GameState::handle(const ThirdSkillUseEvent &event) {
    event.hero_.useSkill(SkillName::ThirdSkill, *this);
}
