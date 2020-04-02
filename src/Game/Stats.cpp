#include "Stats.h"
#include <algorithm>
#include <cassert>

void Stats::changeDamage(int delta) { damage_ += delta; }
void Stats::changeAttackRange(int delta) { attackRange_ += delta; }
void Stats::changeMoveSpeed(int delta) { moveSpeed_ += delta; }
void Stats::changeAttackSpeed(int delta) { attackSpeed_ += delta; }
void Stats::changeMaxHP(int delta) { maxHP_ += delta; }
void Stats::changeMaxMP(int delta) { maxMP_ += delta; }

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
void Stats::changeArmor(int delta) { armor_ += delta; }
void Stats::changeResist(double delta) { resist_ += delta; }

void Stats::refreshStats() {
    manaPoints_ = maxMP_;
    healthPoints_ = maxHP_;
}


// setters and getters

int Stats::getDamage() const { return damage_; }
void Stats::setDamage(int damage) { damage_ = damage; } // forbid negative damage?

unsigned Stats::getAttackRange() const { return attackRange_; }
void Stats::setAttackRange(unsigned attackRange) { attackRange_ = attackRange; }

unsigned Stats::getMoveSpeed() const { return moveSpeed_; }
void Stats::setMoveSpeed(unsigned moveSpeed) { moveSpeed_ = moveSpeed; }

unsigned Stats::getAttackSpeed() const { return attackSpeed_; }
void Stats::setAttackSpeed(unsigned attackSpeed) { attackSpeed_ = attackSpeed; }

unsigned Stats::getMaxHp() const { return maxHP_; }
void Stats::setMaxHp(unsigned maxHp) { maxHP_ = maxHp; }

unsigned Stats::getMaxMp() const { return maxMP_; }
void Stats::setMaxMp(unsigned maxMp) { maxMP_ = maxMp; }

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

int Stats::getArmor() const { return armor_; }
void Stats::setArmor(int armor) { armor_ = armor; }

double Stats::getResist() const { return resist_; }
void Stats::setResist(double resist) {
    assert(resist >= 0 && resist <= 1 && "Wrong new resist!");
    resist_ = resist;
}
