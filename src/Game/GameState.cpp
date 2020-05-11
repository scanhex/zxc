#include "GameState.h"
#include <algorithm>

GameState::GameState(std::vector<Unit *> &units) : units_{units} {
    heroes_.push_back(dynamic_cast<Hero *>(units_[0]));
    heroes_.push_back(dynamic_cast<Hero *>(units_[1]));
}

GameState::GameState() {
    // create 2 default heroes and 2 default creeps
    units_.push_back(new Hero(Player::First));
    units_.push_back(new Hero(Player::Second));
    units_.push_back(new Creep(Team::Radiant));
    units_.push_back(new Creep(Team::Dire));

    heroes_.push_back(dynamic_cast<Hero *>(units_[0]));
    heroes_.push_back(dynamic_cast<Hero *>(units_[1]));
}

Hero *GameState::getHero(Player player) const {
    return dynamic_cast<Hero *>(units_[static_cast<uint8_t>(player)]);
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
    return heroes_[0]->getDeathCounter() >= 2 || heroes_[1]->getDeathCounter() >= 2;
}

void GameState::update(double elapsedTime) { // time in milliseconds
    assert(elapsedTime >= 0);
    double elapsedTimeInSeconds = elapsedTime / 1000.0;

    for (Unit *unit : units_) {
        unit->updateUnit(elapsedTimeInSeconds);
    }
}

void GameState::refreshAllUnits() {
    for (Hero *hero : heroes_) {
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
    Hero *myHero = heroes_[static_cast<uint8_t>(player)];
    // first serialize player(my))Hero
    myHero->serialize(writer);
    // then every other unit
    for (Unit *unit : units_) {
        if (unit != myHero) {
            unit->serialize(writer);
        }
    }
}

void GameState::deserialize(BufferIO::BufferReader &reader, Player player) {
    Hero *myHero = heroes_[static_cast<uint8_t>(player)];
    // first serialize player(my))Hero
    myHero->deserialize(reader);
    // then every other unit
    for (Unit *unit : units_) {
        if (unit != myHero) {
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

void GameState::handle(const FirstSkillUseEvent &event) {
    event.hero_.useSkill(SkillName::FirstSkill, *this);
}

void GameState::handle(const SecondSkillUseEvent &event) {
    event.hero_.useSkill(SkillName::SecondSkill, *this);
}

void GameState::handle(const ThirdSkillUseEvent &event) {
    event.hero_.useSkill(SkillName::ThirdSkill, *this);
}
