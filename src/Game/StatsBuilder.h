#pragma once

#include "Stats.h"

class StatsBuilder {
public:
    StatsBuilder() = default;
    Stats create();

    StatsBuilder &setDamage(int32_t damage);
    StatsBuilder &setAttackRange(uint32_t attackRange);
    StatsBuilder &setMoveSpeed(uint32_t moveSpeed);
    StatsBuilder &setTurnRate(double turnRate);
    StatsBuilder &setAttackSpeed(uint32_t attackSpeed);
    StatsBuilder &setMaxHp(uint32_t maxHp);
    StatsBuilder &setMaxMp(uint32_t maxMp);
    StatsBuilder &setHpRegen(double hpRegen);
    StatsBuilder &setMpRegen(double mpRegen);
    StatsBuilder &setArmor(int32_t armor);
    StatsBuilder &setResist(double resist);
private:
    Stats stats_;
};
