#include <cassert>
#include "GameStateServer.h"

int main() {
    GameStateServer game(1.0 / 30);
    while (!game.gameIsFinished()) {
        game.applyDamage(100, Player::First);
        game.applyDamagePhys(100, Player::Second);
    }
    assert(game.isDead(Player::First));
    assert(!game.isDead(Player::Second));
    return 0;
}
