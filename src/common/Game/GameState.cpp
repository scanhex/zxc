#include "GameState.h"

#include <algorithm>
#include <iostream>

GameState::GameState() {
    // create 2 default heroes and 2 default creeps
    heroes_.push_back(new Hero(Player::First));
    heroes_.push_back(new Hero(Player::Second));

    units_.push_back(heroes_[0]);
    units_.push_back(heroes_[1]);
    units_.push_back(new Creep(Team::Radiant));
    units_.push_back(new Creep(Team::Dire));
}

void GameState::update(double elapsedTime) {  // time in milliseconds
    assert(elapsedTime >= 0);
    double elapsedTimeInSeconds = elapsedTime / 1000.0;

    for (Unit *unit : units_) {
        unit->updateUnit(elapsedTimeInSeconds, units_);
        for (Attack *attack : unit->myAttacks_) {
            attack->update(elapsedTimeInSeconds);
        }
    }
}

void GameState::startNewGame() {
    refreshAllUnits();
    GoldChangedEvent(START_GOLD).fire();
}

void GameState::refreshAllUnits() {
    for (Unit *unit : units_) {
        unit->refresh();
    }
}

void GameState::reverseIndices() {
    for (Unit *unit : units_) {
        unit->unique_id_ = 255 - unit->unique_id_;  // TODO better way?
    }
}

void GameState::serialize(BufferIO::BufferWriter &writer, Player player) {
    Team team = static_cast<Team>(player);
    // first serialize player team
    for (Unit *unit : units_) {
        if (unit->getTeam() == team) {
            unit->serialize(writer);
        }
    }
    // then every other unit
    for (Unit *unit : units_) {
        if (unit->getTeam() != team) {
            unit->serialize(writer);
        }
    }
}

void GameState::deserialize(BufferIO::BufferReader &reader, Player player) {
    Team team = static_cast<Team>(player);
    // first serialize player team
    for (Unit *unit : units_) {
        if (unit->getTeam() == team) {
            unit->deserialize(reader);
        }
    }
    // then every other unit
    for (Unit *unit : units_) {
        if (unit->getTeam() != team) {
            unit->deserialize(reader);
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

void GameState::handle(const AttackEvent &event) {
    Unit *attacker = findUnitByID(event.attackerID_);
    Unit *target = findUnitByID(event.targetID_);
    // assert(attacker && target);
    if (!attacker) {
        std::cerr << "GameState::handle(AttackEvent) -> Attacker was not found. attackerID_ = " << event.attackerID_
                  << std::endl;
    }
    if (!target) {
        std::cerr << "GameState::handle(AttackEvent) -> Target was not found. targetID_ = " << event.targetID_
                  << std::endl;
    }
    for (auto attack : attacker->myAttacks_) {
        if (!attack->getMovingFlag()) {
            attack->setAttacker(attacker);
            attack->setTarget(target);
            attack->setMovingFlag(true);
            attack->setPosition(attacker->getPosition());
            break;
        }
    }
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

// getters and setters

Hero *GameState::getHero(Player player) const {
    return dynamic_cast<Hero *>(units_[static_cast<uint8_t>(player)]);
}

double GameState::getHealthPoints(Player player) const {
    return getHero(player)->getHealthPoints();
}

Point GameState::getPosition(Player player) const {
    return getHero(player)->getPosition();
}

double GameState::getAngle(Player player) const {
    return getHero(player)->getAngle();
}

Point GameState::getDestination(Player player) const {
    return getHero(player)->getDestination();
}

bool GameState::isDead(Player player) const {
    return getHero(player)->isDead();
}

void GameState::setHealthPoints(double amount, Player player) const {
    getHero(player)->setHealthPoints(amount);
}

void GameState::setPosition(Point pos, Player player) const {
    getHero(player)->setPosition(pos);
}

void GameState::setPosition(double x, double y, Player player) const {
    getHero(player)->setPosition(x, y);
}

void GameState::setDestination(double x, double y, Player player) const {
    getHero(player)->setDestination(x, y);
}

void GameState::setAngle(double angle, Player player) const {
    getHero(player)->setAngle(angle);
}

void GameState::applyDamage(double amount, Player player) const {
    getHero(player)->applyDamage(amount);
}

void GameState::applyDamagePhys(double amount, Player player) const {
    getHero(player)->applyDamagePhys(amount);
}

void GameState::applyDamageMagic(double amount, Player player) const {
    getHero(player)->applyDamageMagic(amount);
}

void GameState::regenMana(double amount, Player player) const {
    getHero(player)->regenMana(amount);
}

void GameState::spendMana(double amount, Player player) const {
    getHero(player)->spendMana(amount);
}

bool GameState::canSpendMana(double amount, Player player) const {
    return getHero(player)->canSpendMana(amount);
}

GameStatus GameState::gameStatus() const {
    if (heroes_[0]->getDeathCounter() >= 2) {
        return GameStatus::SecondPlayerWon;
    } else if (heroes_[1]->getDeathCounter() >= 2) {
        return GameStatus::FirstPlayerWon;
    }

    return GameStatus::InProgress;
}

Unit *GameState::findUnitByID(uint8_t id) {
    for (Unit *unit : units_) {
        if (unit->unique_id_ == id) {
            return unit;
        }
    }
    return nullptr;
}

std::vector<Unit *> &GameState::getAllUnits() {
    return units_;
}
