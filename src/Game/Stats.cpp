#include "Stats.h"
#include <algorithm>
#include <cassert>

void Stats::changeDamage(int32_t delta) { damage_ += delta; }
void Stats::changeAttackRange(double delta) { attackRange_ += delta; }
void Stats::changeMoveSpeed(int32_t delta) { moveSpeed_ += delta; }
void Stats::changeTurnRate(int32_t delta) { turnRate_ += delta; }
void Stats::changeAttackSpeed(int32_t delta) { attackSpeed_ += delta; }
void Stats::changeMaxHP(int32_t delta) {
    changeHP(delta * (healthPoints_ / maxHP_));
    maxHP_ += delta;
}
void Stats::changeMaxMP(int32_t delta) {
    changeMP(delta * (manaPoints_ / maxMP_));
    maxMP_ += delta;
}

void Stats::changeHP(double delta) {
    healthPoints_ += delta;
    healthPoints_ = std::clamp(healthPoints_, 0.0, static_cast<double>(maxHP_));
}

void Stats::changeMP(double delta) {
    manaPoints_ += delta;
    manaPoints_ = std::clamp(manaPoints_, 0.0, static_cast<double>(maxMP_));
}

void Stats::changeHPRegen(double delta) { hpRegen_ += delta; }
void Stats::changeMPRegen(double delta) { mpRegen_ += delta; }
void Stats::changeArmor(int32_t delta) { armor_ += delta; }
void Stats::changeResist(double delta) { resist_ += delta; }

void Stats::refreshStats() {
    manaPoints_ = maxMP_;
    healthPoints_ = maxHP_;
}

void Stats::serialize(BufferIO::BufferWriter &writer) const {
    // damage, attackRange, moveSpeed, attackSpeed, turnRate?
    // maxHP, maxMP, manaPoints, hpRegen, mpRegen, armor, resist?
    writer.writeDouble(healthPoints_);
}

void Stats::deserialize(BufferIO::BufferReader &reader) {
    // damage, attackRange, moveSpeed, attackSpeed, turnRate?
    // maxHP, maxMP, manaPoints, hpRegen, mpRegen, armor, resist?
    healthPoints_ = reader.readDouble();
}


// setters and getters

int32_t Stats::getDamage() const { return damage_; }
void Stats::setDamage(int32_t damage) { damage_ = damage; } // forbid negative applyDamage?

int32_t Stats::getDefaultDamage() const { return default_damage_; }
void Stats::setDefaultDamage(int32_t damage) { default_damage_ = damage; }

double Stats::getAttackRange() const { return attackRange_; }
void Stats::setAttackRange(double attackRange) { attackRange_ = attackRange; }

uint32_t Stats::getMoveSpeed() const { return moveSpeed_; }
void Stats::setMoveSpeed(uint32_t moveSpeed) { moveSpeed_ = moveSpeed; }

double Stats::getTurnRate() const { return turnRate_; }
void Stats::setTurnRate(double turnRate) { turnRate_ = turnRate; }

uint32_t Stats::getAttackSpeed() const { return attackSpeed_; }
void Stats::setAttackSpeed(uint32_t attackSpeed) { attackSpeed_ = attackSpeed; }

uint32_t Stats::getMaxHp() const { return maxHP_; }
void Stats::setMaxHp(uint32_t maxHp) {
    healthPoints_ = healthPoints_ * maxHp / maxHP_;
    maxHP_ = maxHp;
}

uint32_t Stats::getMaxMp() const { return maxMP_; }
void Stats::setMaxMp(uint32_t maxMp) {
    manaPoints_ = manaPoints_ * maxMp / maxMP_;
    maxMP_ = maxMp;
}

double Stats::getHealthPoints() const { return healthPoints_; }
void Stats::setHealthPoints(double healthPoints) {
    assert(healthPoints >= 0 && healthPoints <= maxHP_ && "Wrong new health points!");
    healthPoints_ = healthPoints;
}

double Stats::getManaPoints() const { return manaPoints_; }
void Stats::setManaPoints(double manaPoints) {
    assert(manaPoints >= 0 && manaPoints <= maxHP_ && "Wrong new mana points!");
    manaPoints_ = manaPoints;
}

int32_t Stats::getDefaultHealthPoints() const { return default_maxHP_; }
void Stats::setDefaultHealthPoints(int32_t healthPoints) { default_maxHP_ = healthPoints; }

int32_t Stats::getDefaultManaPoints() const { return default_maxMP_; }
void Stats::setDefaultManaPoints(int32_t manaPoints) { default_maxMP_ = manaPoints; }

double Stats::getHpRegen() const { return hpRegen_; }
void Stats::setHpRegen(double hpRegen) { hpRegen_ = hpRegen; }

double Stats::getMpRegen() const { return mpRegen_; }
void Stats::setMpRegen(double mpRegen) { mpRegen_ = mpRegen; }

int32_t Stats::getArmor() const { return armor_; }
void Stats::setArmor(int32_t armor) { armor_ = armor; }

double Stats::getResist() const { return resist_; }
void Stats::setResist(double resist) {
    assert(resist >= 0 && resist <= 1 && "Wrong new resist!");
    resist_ = resist;
}
