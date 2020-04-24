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
            double healPerTick = hero->getHpRegen() * elapsedTimeInSeconds;
            double manaPerTick = hero->getMpRegen() * elapsedTimeInSeconds;
            hero->applyHeal(healPerTick);
            hero->regenMana(manaPerTick);

            Point pos = hero->getPosition();
            Point dest = hero->getDestination();
            if (pos == dest) continue;
            Point vector = dest - pos;
            double angle = std::acos(vector.y_ / vector.vectorLength());
            if (vector.x_ > 0) angle = 2 * M_PI - angle;
            double delta = (hero->getTurnRate() / 100.0) * elapsedTimeInSeconds;
            double myAngle = hero->getAngle();
            if (std::abs(angle - myAngle) <= M_PI) {
                if (std::abs(angle - myAngle) < delta) {
                    hero->setAngle(angle);
                } else {
                    hero->changeAngle(angle > myAngle ? delta : -delta);
                }
            } else {
                if (2 * M_PI - std::abs(angle - myAngle) < delta) {
                    hero->setAngle(angle);
                } else {
                    hero->changeAngle(angle > myAngle ? -delta : delta);
                }
            }

            delta = std::abs(angle - myAngle);
            if (std::min(delta, 2 * M_PI - delta) > 1) continue;


            double factor = (hero->getMoveSpeed() / 100.0) * elapsedTimeInSeconds;
            if (vector.vectorLengthIsLessThan(factor)) {
                hero->setPosition(hero->getDestination());
            } else {
                vector.normalize();
                vector *= factor;
                hero->changePositionBy(vector);
            }
        }
    }
}

void GameState::applyMove(Player player, double x, double y) {
    Hero *hero = getHero(player);
    hero->setDestination(x, y);
}

void GameState::applyEvent(Event event) {
    Hero *hero = getHero(event.player_);

    switch (event.eventName_) {
        case EventName::firstSkill:
            hero->useSkill(SkillNum::first, *this);
            break;
        case EventName::secondSkill:
            hero->useSkill(SkillNum::second, *this);
            break;
        case EventName::thirdSkill:
            hero->useSkill(SkillNum::third, *this);
            break;
        case EventName::move:
            applyMove(event.player_, event.x_, event.y_);
            break;
        default:
            assert(false);
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

