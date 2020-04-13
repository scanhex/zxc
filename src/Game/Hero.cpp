#include "Hero.h"
#include <algorithm>

Hero::Hero(Stats stats, Point position, Player player) : Unit(stats, position),
                                                         player_{player},
                                                         gold_{START_GOLD},
                                                         level_{1},
                                                         experience_{0} {
    skills_.emplace_back(player_, SkillNum::first);
    skills_.emplace_back(player_, SkillNum::second);
    skills_.emplace_back(player_, SkillNum::third);
}

bool Hero::canSpendGold(uint32_t amount) {
    return gold_ >= amount;
}

void Hero::addGold(uint32_t amount) {
    gold_ += amount;
}

void Hero::spendGold(uint32_t amount) {
    assert(canSpendGold(amount) && "Not enough gold to spend!");
    gold_ -= amount;
}

void Hero::levelUp(uint32_t delta) { level_ = std::min(level_ + delta, MAX_LEVEL); }

void Hero::incrementLevel() { levelUp(1); }

void Hero::changeExperience(uint32_t delta) {
    experience_ += delta;
    if (level_ == MAX_LEVEL) {
        experience_ = std::min(experience_, EXP_PER_LEVEL);
    } else if (experience_ >= EXP_PER_LEVEL) {
        experience_ -= EXP_PER_LEVEL;
        incrementLevel();
    }
}

void Hero::useSkill(SkillNum skillNum, GameState &gameState) {
    switch (skillNum){
        case SkillNum::first:
            skills_[0].handleEvent(gameState);
            break;
        case SkillNum::second:
            skills_[1].handleEvent(gameState);
            break;
        case SkillNum::third:
            skills_[2].handleEvent(gameState);
            break;
        default:
            assert(false);
    }
}


// setters and getters

uint32_t Hero::getGold() const { return gold_; }
void Hero::setGold(uint32_t gold) { gold_ = gold; }

uint32_t Hero::getLevel() const { return level_; }
void Hero::setLevel(uint32_t level) {
    assert(level >= 1 && level <= MAX_LEVEL && "Wrong new level!");
    level_ = level;
}

uint32_t Hero::getExperience() const { return experience_; }
void Hero::setExperience(uint32_t experience) {
    assert(experience < EXP_PER_LEVEL && "Wrong new experience!");
    experience_ = experience;
}
