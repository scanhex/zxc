#include "GameStateHandler.h"
#include "GameState.h"

GameStateHandler::GameStateHandler(GameState &gameState) : gameState_{gameState} {}

void GameStateHandler::handle(const MoveEvent &event) {
    event.hero_.setDestination(event.x_, event.y_);
}

void GameStateHandler::handle(const ShortCoilUseEvent &event) {
    // TODO now loops over 2 heroes
    for (Hero *hero : {gameState_.firstHero_, gameState_.secondHero_}) {
        if (hero != &event.hero_) {
            hero->applyDamage(10);
        }
    }
}

void GameStateHandler::handle(const MidCoilUseEvent &event) {
    // TODO now loops over 2 heroes
    for (Hero *hero : {gameState_.firstHero_, gameState_.secondHero_}) {
        if (hero != &event.hero_) {
            hero->applyDamage(20);
        }
    }
}

void GameStateHandler::handle(const LongCoilUseEvent &event) {
    // TODO now loops over 2 heroes
    for (Hero *hero : {gameState_.firstHero_, gameState_.secondHero_}) {
        if (hero != &event.hero_) {
            hero->applyDamage(30);
        }
    }
}
