#pragma once

#include "Stats.h"

class StatsBuilder {
public:
    StatsBuilder() = default;
    Stats create();

    StatsBuilder &setDamage(int damage);
    StatsBuilder &setAttackRange(unsigned attackRange);
    StatsBuilder &setMoveSpeed(unsigned moveSpeed);
    StatsBuilder &setAttackSpeed(unsigned attackSpeed);
    StatsBuilder &setMaxHp(unsigned maxHp);
    StatsBuilder &setMaxMp(unsigned maxMp);
    StatsBuilder &setHpRegen(double hpRegen);
    StatsBuilder &setMpRegen(double mpRegen);
    StatsBuilder &setArmor(int armor);
    StatsBuilder &setResist(double resist);
private:
    Stats stats_;
};