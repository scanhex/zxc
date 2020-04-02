#include <iostream>
#include <cassert>
#include "Hero.h"
#include "StatsBuilder.h"


int main() {
    // here for now
    StatsBuilder heroStatsBuilder = StatsBuilder().
            setDamage(100).
            setAttackRange(100).
            setMoveSpeed(350).
            setAttackSpeed(100).
            setMaxHp(1000).
            setMaxMp(300).
            setHpRegen(2).
            setMpRegen(1).
            setArmor(3).
            setResist(0.25);
    Hero chel(heroStatsBuilder.create(), Point(100, 100));
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