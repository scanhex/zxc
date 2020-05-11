#define _USE_MATH_DEFINES

#include <algorithm>
#include <cmath>
#include "Hero.h"
#include "GameState.h"

StatsBuilder Hero::defaultHeroStatsBuilder_ =
        StatsBuilder()
                .setDamage(100)
                .setAttackRange(100)
                .setMoveSpeed(450)
                .setTurnRate(0.5)
                .setAttackSpeed(100)
                .setMaxHp(1000)
                .setMaxMp(300)
                .setHpRegen(1.5)
                .setMpRegen(1)
                .setArmor(3)
                .setResist(0.25);

Position Hero::heroSpawns_[] = {Position(Point(-6, -6), 0), // First
                                Position(Point(6, 6), M_PI)};     // Second

Hero::Hero(Player player) : Unit(defaultHeroStatsBuilder_.create(), heroSpawns_[static_cast<uint8_t>(player)]),
                            gold_{START_GOLD},
                            level_{1},
                            experience_{0},
                            deathCounter_{0},
                            skills_{ShortCoil(*this),
                                    MidCoil(*this),
                                    LongCoil(*this)} {
    team_ = static_cast<Team>(player);
    killReward_ = 500;
}

Hero::Hero(Player player, Position position) : Unit(defaultHeroStatsBuilder_.create(), position),
                                               gold_{START_GOLD},
                                               level_{1},
                                               experience_{0},
                                               deathCounter_{0},
                                               skills_{ShortCoil(*this),
                                                       MidCoil(*this),
                                                       LongCoil(*this)} {
    team_ = static_cast<Team>(player);
}

Hero::Hero(Player player, Position position, Stats stats) : Hero(player, position) {
    stats_ = stats;
}

bool Hero::canSpendGold(uint32_t amount) const {
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

bool Hero::isSkillReady(SkillName skillName) {
    return skills_[static_cast<uint8_t>(skillName)].isReady();
}

void Hero::useSkill(SkillName skillName, GameState &gameState) {
    skills_[static_cast<uint8_t>(skillName)].use(gameState);
}

void Hero::updateUnit(double elapsedTimeInSeconds) {
    if (isDead()) {
        if (deathCounter_ < 1) {
            ++deathCounter_;
            refreshUnit();
        }
    } else {
        Unit::updateUnit(elapsedTimeInSeconds);
        for (Coil &coil : skills_) {
            coil.update(elapsedTimeInSeconds);
        }
    }
}

void Hero::refreshPosition() {
    position_ = heroSpawns_[static_cast<uint8_t>(team_)];
}

void Hero::refreshUnit() {
    Unit::refreshUnit();
    refreshPosition();
    for (Coil &coil : skills_) {
        coil.refreshCoolDown();
    }
}

void Hero::serialize(BufferIO::BufferWriter &writer) {
    Unit::serialize(writer);
    writer.writeInt32(gold_); // TODO add UINT RUslan?
    // lvl, exp?

}

void Hero::deserialize(BufferIO::BufferReader &reader) {
    Unit::deserialize(reader);
    gold_ = reader.readInt32(); // TODO add UINT RUslan?
    // lvl, exp?
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

uint32_t Hero::getDeathCounter() const {
    return deathCounter_;
}

void Hero::setDeathCounter(uint32_t number) {
    deathCounter_ = number;
}
