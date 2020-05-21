#pragma once

#include "Unit.h"
#include "StatsBuilder.h"

class Creep : public Unit {
private:
    static StatsBuilder defaultCreepStatsBuilder_;
    static Position creepSpawns_[NUM_TEAMS];

    Point findNewDestination(std::vector<Unit *> &allUnits);

    void updateUnit(double elapsedTimeInSeconds, std::vector<Unit *> &allUnits) override;
    void refreshPosition();

public:
    explicit Creep(Team team);
};
