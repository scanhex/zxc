#include "Hero.h"

Hero::Hero(int damage, int attackRange, int moveSpeed, int attackSpeed, int maxHP,
           int maxMP, int hpRegen, int mpRegen, int armor, double resist, Point position) :
        Unit(damage, attackRange, moveSpeed, attackSpeed, maxHP, maxMP, hpRegen, mpRegen, armor, resist, position),
        gold_{START_GOLD}, level_{1}, experience_{0} {}

void Hero::changeGold(int delta) { gold_ += delta; }

void Hero::changeLevel(int delta) { level_ = std::min(level_ + delta, MAX_LEVEL); }

void Hero::changeLevelTo(int newLevel) {
    assert(newLevel >= 1 && newLevel <= MAX_LEVEL && "Wrong new level!");

    level_ = newLevel;
}

void Hero::incrLevel() { changeLevel(1); }

void Hero::changeExperience(int delta) {
    experience_ += delta;
    if(level_ == MAX_LEVEL){
        experience_ = std::min(experience_, EXP_PER_LEVEL);
    }else if (experience_ >= EXP_PER_LEVEL) {
        experience_ -= EXP_PER_LEVEL;
        incrLevel();
    }
}
