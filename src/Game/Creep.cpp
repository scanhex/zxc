#define _USE_MATH_DEFINES

#include "Creep.h"
#include <cmath>

StatsBuilder Creep::defaultCreepStatsBuilder_ =
        StatsBuilder()
                .setDamage(20)
                .setAttackRange(100)
                .setMoveSpeed(450)
                .setTurnRate(0.2)
                .setAttackSpeed(50)
                .setMaxHp(300)
                .setMaxMp(300)
                .setHpRegen(1.5)
                .setMpRegen(1)
                .setArmor(3)
                .setResist(0.25);


Position Creep::creepSpawns_[] = {Position(Point(-3, -6), 0),  // Radiant
                                  Position(Point(3, 6), M_PI), // Dire
                                  Position(Point(0, 0), 0)};   // Neutral

Creep::Creep(Team team) : Unit(defaultCreepStatsBuilder_.create(), creepSpawns_[static_cast<uint8_t>(team)]) {
    team_ = team;
    killReward_ = 100;
}
