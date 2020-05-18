#include "Skills.h"

#include <algorithm>

#include "Events/Events.h"
#include "GameState.h"

Skill::Skill(Hero &hero) : hero_{hero} {}

Coil::Coil(Hero &hero, double len, double radius, double damage) : Skill(hero),
                                                                   coolDown_{0},
                                                                   len_{len},
                                                                   radius_{radius},
                                                                   damage_{damage} {}

void Coil::use(GameState &gameState) {
    //assert(coolDown_ == 0.0);
    /*
     * Мы решили пока что доверять клиенту, что кд прошел,
     * иначе даже минимальную рассинхонизацию подсчета времени контролировать сложно,
     * и могут возникнуть проблемы с отрисовкой того, что не произошло.
     */
    auto team = hero_.getTeam();
    Point coilCenter = hero_.shiftUnitPosition(len_);
    for (Unit *unit : gameState.units_) {
        if (unit->getTeam() != team && !unit->isDead() && unit->inRadius(coilCenter, radius_)) {
            unit->applyDamage(damage_);
            if (unit->isDead()) {
                hero_.claimReward(unit);
            }
        }
    }

    coolDown_ = COOL_DOWN;
}

void Coil::update(double elapsedTimeInSeconds) {
    coolDown_ = std::max(0.0, coolDown_ - elapsedTimeInSeconds);
}

void Coil::refreshCoolDown() {
    coolDown_ = 0.0;
}

bool Coil::isReady() const {
    return coolDown_ == 0.0;
}

ShortCoil::ShortCoil(Hero &hero) : Coil(hero, LEN, RADIUS, DAMAGE) {}
MidCoil::MidCoil(Hero &hero) : Coil(hero, LEN, RADIUS, DAMAGE) {}
LongCoil::LongCoil(Hero &hero) : Coil(hero, LEN, RADIUS, DAMAGE) {}
