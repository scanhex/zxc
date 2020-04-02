#include "StatsBuilder.h"

Stats StatsBuilder::create() {
    return stats_;
}

StatsBuilder &StatsBuilder::setDamage(int32_t damage) {
    stats_.setDamage(damage);
    return *this;
}

StatsBuilder &StatsBuilder::setAttackRange(uint32_t  attackRange) {
    stats_.setAttackRange(attackRange);
    return *this;
}

StatsBuilder &StatsBuilder::setMoveSpeed(uint32_t  moveSpeed) {
    stats_.setMoveSpeed(moveSpeed);
    return *this;
}

StatsBuilder &StatsBuilder::setAttackSpeed(uint32_t  attackSpeed) {
    stats_.setAttackSpeed(attackSpeed);
    return *this;
}

StatsBuilder &StatsBuilder::setMaxHp(uint32_t  maxHp) {
    stats_.setMaxHp(maxHp);
    return *this;
}

StatsBuilder &StatsBuilder::setMaxMp(uint32_t  maxMp) {
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

StatsBuilder &StatsBuilder::setArmor(int32_t armor) {
    stats_.setArmor(armor);
    return *this;
}

StatsBuilder &StatsBuilder::setResist(double resist) {
    stats_.setResist(resist);
    return *this;
}
