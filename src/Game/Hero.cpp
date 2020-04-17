#include "Hero.h"
#include <algorithm>

StatsBuilder Hero::defaultHeroStatsBuilder =
        StatsBuilder().
                setDamage(100).
                setAttackRange(100).
                setMoveSpeed(350).
                setAttackSpeed(100).
                setMaxHp(1000).
                setMaxMp(300).
                setHpRegen(0.1).
                setMpRegen(1).
                setArmor(3).
                setResist(0.25);

Point Hero::firstHeroStartingPoint = Point(-6, -6);
Point Hero::secondHeroStartingPoint = Point(6, 6);

Hero::Hero(Player player) : Unit(defaultHeroStatsBuilder.create(),
                                 player == Player::First ?
                                 firstHeroStartingPoint :
                                 secondHeroStartingPoint),
                            player_{player},
                            gold_{START_GOLD},
                            level_{1},
                            experience_{0},
                            skills_{Skill(player_, SkillNum::first),
                                    Skill(player_, SkillNum::second),
                                    Skill(player_, SkillNum::third)} {}

Hero::Hero(Player player, Point position) : Unit(defaultHeroStatsBuilder.create(), position),
                                            player_{player},
                                            gold_{START_GOLD},
                                            level_{1},
                                            experience_{0},
                                            skills_{Skill(player_, SkillNum::first),
                                                    Skill(player_, SkillNum::second),
                                                    Skill(player_, SkillNum::third)} {}

Hero::Hero(Player player, Point position, Stats stats) : Hero(player, position) {
    stats_ = stats;
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
    switch (skillNum) {
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
