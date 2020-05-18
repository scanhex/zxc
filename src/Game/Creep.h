#pragma once

#include "Unit.h"
#include "StatsBuilder.h"

class Creep : public Unit {
private:
    static StatsBuilder defaultCreepStatsBuilder_;
    static Position creepSpawns_[NUM_TEAMS];

    void updateUnit(double elapsedTimeInSeconds, std::vector<Unit * >& allUnits) override;

public:
    explicit Creep(Team team);
};

