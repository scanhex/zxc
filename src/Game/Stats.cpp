#include "Stats.h"
#include <algorithm>
#include <cassert>

void Stats::changeDamage(int32_t delta) { damage_ += delta; }
void Stats::changeAttackRange(int32_t delta) { attackRange_ += delta; }
void Stats::changeMoveSpeed(int32_t delta) { moveSpeed_ += delta; }
void Stats::changeAttackSpeed(int32_t delta) { attackSpeed_ += delta; }
void Stats::changeMaxHP(int32_t delta) { maxHP_ += delta; }
void Stats::changeMaxMP(int32_t delta) { maxMP_ += delta; }

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


// setters and getters

int32_t Stats::getDamage() const { return damage_; }
void Stats::setDamage(int32_t damage) { damage_ = damage; } // forbid negative damage?

uint32_t Stats::getAttackRange() const { return attackRange_; }
void Stats::setAttackRange(uint32_t attackRange) { attackRange_ = attackRange; }

uint32_t Stats::getMoveSpeed() const { return moveSpeed_; }
void Stats::setMoveSpeed(uint32_t moveSpeed) { moveSpeed_ = moveSpeed; }

uint32_t Stats::getAttackSpeed() const { return attackSpeed_; }
void Stats::setAttackSpeed(uint32_t attackSpeed) { attackSpeed_ = attackSpeed; }

uint32_t Stats::getMaxHp() const { return maxHP_; }
void Stats::setMaxHp(uint32_t maxHp) { maxHP_ = maxHp; }

uint32_t Stats::getMaxMp() const { return maxMP_; }
void Stats::setMaxMp(uint32_t maxMp) { maxMP_ = maxMp; }

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
