#include "GameState.h"
#include "GameStateServer.h"

GameState::GameState(Hero &firstHero_, Hero &secondHero_) : firstHero{firstHero_}, secondHero{secondHero_} {}

double GameState::getHealthPoints(Player player) {
    switch (player) {
        case Player::First:
            return firstHero.getHealthPoints();
        case Player::Second:
            return secondHero.getHealthPoints();
        default:
            assert(false);
    }
}

void GameState::setHealthPoints(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero.setHealthPoints(amount);
            break;
        case Player::Second:
            secondHero.setHealthPoints(amount);
            break;
        default:
            assert(false);
    }
}

bool GameState::gameIsFinished() {
    return firstHero.isDead() || secondHero.isDead();
}


GameStateServer::GameStateServer(Hero &firstHero_, Hero &secondHero_, double gameTick) : GameState(firstHero_,
                                                                                                   secondHero_),
                                                                                         gameTick_{gameTick} {}

void GameStateServer::update() {
    // only regen for now
    double healPerTick = firstHero.getHpRegen() * gameTick_;
    double manaPerTick = firstHero.getMpRegen() * gameTick_;
    firstHero.applyHeal(healPerTick);
    firstHero.regenMana(manaPerTick);

    healPerTick = secondHero.getHpRegen() * gameTick_;
    manaPerTick = secondHero.getMpRegen() * gameTick_;
    secondHero.applyHeal(healPerTick);
    secondHero.regenMana(manaPerTick);
}

void GameStateServer::applySkill(Player player, SkillNum skillNum) {
    // FOR NOW JUST DAMAGES OTHER PLAYER
    double damage = 0;
    switch (skillNum) {
        case SkillNum::First:
            damage = 10;
            break;
        case SkillNum::Second:
            damage = 20;
            break;
        case SkillNum::Third:
            damage = 30;
            break;
        default:
            assert(false);
    }
    switch (player) {
        case Player::First:
            secondHero.applyDamage(damage);
            break;
        case Player::Second:
            firstHero.applyDamage(damage);
            break;
        default:
            assert(false);
    }
}

void GameStateServer::applyMove(Player player, int32_t x, int32_t y) {
    // ----
}


void GameStateServer::applyDamage(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero.applyDamage(amount);
            break;
        case Player::Second:
            secondHero.applyDamage(amount);
            break;
        default:
            assert(false);
    }
}

void GameStateServer::applyDamagePhys(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero.applyDamagePhys(amount);
            break;
        case Player::Second:
            secondHero.applyDamagePhys(amount);
            break;
        default:
            assert(false);
    }
}

void GameStateServer::applyDamageMagic(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero.applyDamageMagic(amount);
            break;
        case Player::Second:
            secondHero.applyDamageMagic(amount);
            break;
        default:
            assert(false);
    }
}

void GameStateServer::regenMana(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero.regenMana(amount);
            break;
        case Player::Second:
            secondHero.regenMana(amount);
            break;
        default:
            assert(false);
    }
}

void GameStateServer::spendMana(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero.spendMana(amount);
            break;
        case Player::Second:
            secondHero.spendMana(amount);
            break;
        default:
            assert(false);
    }
}

bool GameStateServer::canSpendMana(double amount, Player player) const {
    switch (player) {
        case Player::First:
            return firstHero.canSpendMana(amount);
        case Player::Second:
            return secondHero.canSpendMana(amount);
        default:
            assert(false);
    }
}

bool GameStateServer::isDead(Player player) const {
    switch (player) {
        case Player::First:
            return firstHero.isDead();
        case Player::Second:
            return secondHero.isDead();
        default:
            assert(false);
    }
}
