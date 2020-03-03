#include <iostream>
#include "Hero.h"


int main() {
    Hero chel(100, 100, 350, 100, 1000, 300, 2, 1, 3, 0.25, Point(100, 100));
    while (chel.getLevel() < MAX_LEVEL || chel.getExperience() < EXP_PER_LEVEL) {
        chel.changeExperience(220);
        std::cout << chel.getLevel() << ' ' << chel.getExperience() << '\n';
    }
    std::cout << '\n';
    chel.damagePhys(999);
    std::cout << chel.isDead() << '\n';
    chel.damage(50);
    std::cout << chel.isDead() << '\n';
    chel.damage(150);
    std::cout << chel.isDead() << '\n';
}