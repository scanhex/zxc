#include "Hero.h"
#include <algorithm>

Hero::Hero(Stats stats, Point position) : Unit(stats, position),
                                          gold_{START_GOLD},
                                          level_{1},
                                          experience_{0} {}

bool Hero::canSpendGold(unsigned amount) {
    return gold_ >= amount;
}

void Hero::addGold(unsigned amount) {
    gold_ += amount;
}

void Hero::spendGold(unsigned amount) {
    assert(canSpendGold(amount) && "Not enough gold to spend!");
    gold_ -= amount;
}

void Hero::levelUp(unsigned delta) { level_ = std::min(level_ + delta, MAX_LEVEL); }

void Hero::incrementLevel() { levelUp(1); }

void Hero::changeExperience(unsigned delta) {
    experience_ += delta;
    if (level_ == MAX_LEVEL) {
        experience_ = std::min(experience_, EXP_PER_LEVEL);
    } else if (experience_ >= EXP_PER_LEVEL) {
        experience_ -= EXP_PER_LEVEL;
        incrementLevel();
    }
}


// setters and getters

unsigned Hero::getGold() const { return gold_; }
void Hero::setGold(unsigned gold) { gold_ = gold; }

unsigned Hero::getLevel() const { return level_; }
void Hero::setLevel(unsigned level) {
    assert(level >= 1 && level <= MAX_LEVEL && "Wrong new level!");
    level_ = level;
}

unsigned Hero::getExperience() const { return experience_; }
void Hero::setExperience(unsigned experience) {
    assert(experience < EXP_PER_LEVEL && "Wrong new experience!");
    experience_ = experience;
}
