#include "Hero.h"
#include <algorithm>

Hero::Hero(int damage, unsigned attackRange, unsigned moveSpeed, unsigned attackSpeed, unsigned maxHP,
           unsigned maxMP, double hpRegen, double mpRegen, int armor, double resist, Point position) :
        Unit(damage, attackRange, moveSpeed, attackSpeed, maxHP, maxMP, hpRegen, mpRegen, armor, resist, position),
        gold_{START_GOLD}, level_{1}, experience_{0} {}

void Hero::changeGold(int delta) { gold_ += delta; }

void Hero::levelUp(unsigned delta) { level_ = std::min(level_ + delta, MAX_LEVEL); }

void Hero::incrLevel() { levelUp(1); }

void Hero::getExperience(unsigned delta) {
    experience_ += delta;
    if (level_ == MAX_LEVEL) {
        experience_ = std::min(experience_, EXP_PER_LEVEL);
    } else if (experience_ >= EXP_PER_LEVEL) {
        experience_ -= EXP_PER_LEVEL;
        incrLevel();
    }
}

unsigned Hero::getGold() const {
    return gold_;
}

void Hero::setGold(unsigned gold) {
    gold_ = gold;
}

unsigned Hero::getLevel() const {
    return level_;
}

void Hero::setLevel(unsigned level) {
    assert(level >= 1 && level <= MAX_LEVEL && "Wrong new level!");

    level_ = level;
}

unsigned Hero::getExperience() const {
    return experience_;
}

void Hero::setExperience(unsigned experience) {
    assert(experience >= 0 && experience < EXP_PER_LEVEL && "Wrong new experience!");

    experience_ = experience;
}
