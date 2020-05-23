#define _USE_MATH_DEFINES

#include "Creep.h"
#include "Events/Events.h"
#include <cmath>
#include <cfloat>

StatsBuilder Creep::defaultCreepStatsBuilder_ =
        StatsBuilder()
                .setDamage(20)
                .setAttackRange(1)
                .setMoveSpeed(300)
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
    goldKillReward_ = 100;
    expKillReward_ = 1000;
    giveId();
}

void Creep::refreshPosition() {
    position_ = creepSpawns_[static_cast<uint8_t>(team_)];
}

void Creep::updateUnit(double elapsedTimeInSeconds, std::vector<Unit *> &allUnits) {
    if (isDead()) {
        refreshUnit();
        refreshPosition();
    }
    if (!allUnits.empty()) {
        Unit *closest = findClosestUnit(allUnits);
        position_.setDestination(closest->getPosition());
        if (Point::getDistance(closest->getPosition(), getPosition()) < getAttackRange()) {
            if (Attack *currentAttack = attack(closest)) {
                EventHandler<AttackEvent>::fireEvent(AttackEvent(currentAttack->getAttacker()->unique_id_,
                                                                 currentAttack->getTarget()->unique_id_));
            }
        }
    }
    Unit::updateUnit(elapsedTimeInSeconds, allUnits);
}

Unit *Creep::findClosestUnit(std::vector<Unit *> &allUnits) {
    assert(!allUnits.empty());
    const Point &myPosition = position_.getPosition();
    double closest = DBL_MAX; // ......nevazhno
    Unit *closestUnit = this;
    for (Unit *unit : allUnits) {
        if (unit->getTeam() == team_ || unit->isDead()) continue;
        double current = Point::getDistance(unit->getPosition(), myPosition);
        if (current < closest) {
            closest = current;
            closestUnit = unit;
        }
    }
    return closestUnit;
}

