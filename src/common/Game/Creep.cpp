#define _USE_MATH_DEFINES

#include "Creep.h"

#include <algorithm>
#include <cfloat>
#include <cmath>

#include "Events/Events.h"

StatsBuilder Creep::defaultCreepStatsBuilder_ = StatsBuilder()
                                                    .setDamage(35)
                                                    .setAttackRange(0.1)
                                                    .setMoveSpeed(300)
                                                    .setTurnRate(0.2)
                                                    .setAttackSpeed(50)
                                                    .setMaxHp(300)
                                                    .setMaxMp(300)
                                                    .setHpRegen(1.5)
                                                    .setMpRegen(1)
                                                    .setArmor(3)
                                                    .setResist(0.25);

Position Creep::creepSpawns_[] = {
    Position(Point(-3, -6), 0),   // Radiant
    Position(Point(3, 6), M_PI),  // Dire
    Position(Point(0, 0), 0),     // Neutral
};

Creep::Creep(Team team) : Unit(defaultCreepStatsBuilder_.create(), creepSpawns_[static_cast<uint8_t>(team)]) {
    team_ = team;
    goldKillReward_ = 20;
    expKillReward_ = 1000;
    respawnTime_ = CREEP_RESPAWN_TIME;
    giveId();
}

bool Creep::isHero() {
    return false;
}

void Creep::updateUnit(double elapsedTimeInSeconds, std::vector<Unit *> &allUnits) {
    if (isDead()) {
        moved_ = true;
        position_.setPosition(1000, 1000);
        position_.setDestination(1000, 1000);
        respawnTime_ = std::max(0.0, respawnTime_ - elapsedTimeInSeconds);
        if (respawnTime_ == 0) {
            respawn();
            respawnTime_ = CREEP_RESPAWN_TIME;
        }
        return;
    }
    if (!allUnits.empty()) {
        Unit *closest = findClosestUnit(allUnits);
        position_.setDestination(closest->getPosition());
        if (Point::getDistance(closest->getPosition(), getPosition()) <
            std::max(getHeroRadius() + closest->getHeroRadius(), getAttackRange())) {
            if (Attack *currentAttack = attack(closest)) {
                EventHandler<AttackEvent>::fireEvent(
                    AttackEvent(currentAttack->getAttacker()->unique_id_, currentAttack->getTarget()->unique_id_)
                );
            }
        }
    }
    Unit::updateUnit(elapsedTimeInSeconds, allUnits);
}

Unit *Creep::findClosestUnit(std::vector<Unit *> &allUnits) {
    assert(!allUnits.empty());
    const Point &myPosition = position_.getPosition();
    auto closest = DBL_MAX;  // ......nevazhno
    Unit *closestUnit = this;
    for (Unit *unit : allUnits) {
        if (unit->getTeam() == team_ || unit->isDead()) {
            continue;
        }
        double current = Point::getDistance(unit->getPosition(), myPosition);
        if (current < closest) {
            closest = current;
            closestUnit = unit;
        }
    }
    return closestUnit;
}
