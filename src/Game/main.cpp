#include <iostream>
#include <cassert>
#include "GameState.h"
#include "Hero.h"
#include "StatsBuilder.h"


int main() {
    GameState(1.0 / 30);
    while (chel.getLevel() < MAX_LEVEL || chel.getExperience() < EXP_PER_LEVEL) {
        chel.changeExperience(220);
        std::cout << chel.getLevel() << ' ' << chel.getExperience() << '\n';
    }
    std::cout << '\n';
    chel.damagePhys(999);
    assert(!chel.isDead());
    chel.damage(50);
    assert(!chel.isDead());
    chel.damage(150);
    assert(chel.isDead());
}
