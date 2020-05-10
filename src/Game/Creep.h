#pragma once

#include "Unit.h"
#include "StatsBuilder.h"

class Creep : public Unit {
private:
    static StatsBuilder defaultCreepStatsBuilder_;
    static Position creepSpawns_[NUM_TEAMS];

public:
    explicit Creep(Team team);
};

