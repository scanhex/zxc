#include "StatsBuilder.h"

Stats StatsBuilder::create() {
    return stats_;
}

StatsBuilder &StatsBuilder::setDamage(int damage) {
    stats_.setDamage(damage);
    return *this;
}

StatsBuilder &StatsBuilder::setAttackRange(unsigned attackRange) {
    stats_.setAttackRange(attackRange);
    return *this;
}

StatsBuilder &StatsBuilder::setMoveSpeed(unsigned moveSpeed) {
    stats_.setMoveSpeed(moveSpeed);
    return *this;
}

StatsBuilder &StatsBuilder::setAttackSpeed(unsigned attackSpeed) {
    stats_.setAttackSpeed(attackSpeed);
    return *this;
}

StatsBuilder &StatsBuilder::setMaxHp(unsigned maxHp) {
    stats_.setMaxHp(maxHp);
    return *this;
}

StatsBuilder &StatsBuilder::setMaxMp(unsigned maxMp) {
    stats_.setMaxMp(maxMp);
    return *this;
}

StatsBuilder &StatsBuilder::setHpRegen(double hpRegen) {
    stats_.setHpRegen(hpRegen);
    return *this;
}

StatsBuilder &StatsBuilder::setMpRegen(double mpRegen) {
    stats_.setMpRegen(mpRegen);
    return *this;
}

StatsBuilder &StatsBuilder::setArmor(int armor) {
    stats_.setArmor(armor);
    return *this;
}

StatsBuilder &StatsBuilder::setResist(double resist) {
    stats_.setResist(resist);
    return *this;
}
