#include "GameState.h"

GameState::GameState(Hero &firstHero_, Hero &secondHero_, double gameTick) : gameTick_{gameTick},
                                                                             firstHero{firstHero_},
                                                                             secondHero{secondHero_} {}

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

void GameState::update() {
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

void GameState::applyMove(Player player, int32_t x, int32_t y) {
    // ----
}

void GameState::applyEvent(Player player, EventName eventName) {
    Hero &hero = firstHero;
    if(player == Player::Second) hero = secondHero;

    switch (eventName) {
        case EventName::firstSkill:
            hero.useSkill(SkillNum::first, *this);
            break;
        case EventName::secondSkill:
            hero.useSkill(SkillNum::second, *this);
            break;
        case EventName::thirdSkill:
            hero.useSkill(SkillNum::third, *this);
            break;
        default:
            assert(false);
    }
}

void GameState::applyDamage(double amount, Player player) {
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

void GameState::applyDamagePhys(double amount, Player player) {
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

void GameState::applyDamageMagic(double amount, Player player) {
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

void GameState::regenMana(double amount, Player player) {
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

void GameState::spendMana(double amount, Player player) {
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

bool GameState::canSpendMana(double amount, Player player) const {
    switch (player) {
        case Player::First:
            return firstHero.canSpendMana(amount);
        case Player::Second:
            return secondHero.canSpendMana(amount);
        default:
            assert(false);
    }
}

bool GameState::isDead(Player player) const {
    switch (player) {
        case Player::First:
            return firstHero.isDead();
        case Player::Second:
            return secondHero.isDead();
        default:
            assert(false);
    }
}
