#include "GameState.h"

GameState::GameState(Hero &firstHero, Hero &secondHero) : firstHero_{&firstHero},
                                                          secondHero_{&secondHero} {}

GameState::GameState() {
    // 2 default heros
    firstHero_ = new Hero(Player::First);
    secondHero_ = new Hero(Player::Second);
}

Hero *GameState::getHero(Player player) const {
    switch (player) {
        case Player::First:
            return firstHero_;
        case Player::Second:
            return secondHero_;
        default:
            assert(false);
    }
}

double GameState::getHealthPoints(Player player) const {
    Hero *hero = getHero(player);
    return hero->getHealthPoints();
}

Point GameState::getPosition(Player player) const {
    Hero *hero = getHero(player);
    return hero->getPosition();
}

void GameState::setPosition(Point pos, Player player) {
    Hero *hero = getHero(player);
    hero->setPosition(pos);
}

void GameState::setPosition(double x, double y, Player player) {
    Hero *hero = getHero(player);
    hero->setPosition(x, y);
}

void GameState::setHealthPoints(double amount, Player player) {
    Hero *hero = getHero(player);
    hero->setHealthPoints(amount);
}

bool GameState::gameIsFinished() const {
    return firstHero_->isDead() || secondHero_->isDead();
}

void GameState::update(double elapsedTime) {
    assert(elapsedTime >= 0);
    elapsedTime /= 1000.0;
    // only regen for now
    if (!firstHero_->isDead()) {
        double healPerTick = firstHero_->getHpRegen() * elapsedTime;
        double manaPerTick = firstHero_->getMpRegen() * elapsedTime;
        firstHero_->applyHeal(healPerTick);
        firstHero_->regenMana(manaPerTick);
    }

    if (!secondHero_->isDead()) {
        double healPerTick = secondHero_->getHpRegen() * elapsedTime;
        double manaPerTick = secondHero_->getMpRegen() * elapsedTime;
        secondHero_->applyHeal(healPerTick);
        secondHero_->regenMana(manaPerTick);
    }
}

void GameState::applyMove(Player player, double x, double y) {
    // TODO blink -> move
    Hero *hero = getHero(player);
    hero->setPosition(x, y);
}

void GameState::applyEvent(Event event) {
    Hero *hero = getHero(event.player_);

    switch (event.eventName_) {
        case EventName::firstSkill:
            hero->useSkill(SkillNum::first, *this);
            break;
        case EventName::secondSkill:
            hero->useSkill(SkillNum::second, *this);
            break;
        case EventName::thirdSkill:
            hero->useSkill(SkillNum::third, *this);
            break;
        case EventName::move:
            applyMove(event.player_, event.x_, event.y_);
            break;
        default:
            assert(false);
    }
}

void GameState::applyDamage(double amount, Player player) {
    Hero *hero = getHero(player);
    hero->applyDamage(amount);
}

void GameState::applyDamagePhys(double amount, Player player) {
    Hero *hero = getHero(player);
    hero->applyDamagePhys(amount);
}

void GameState::applyDamageMagic(double amount, Player player) {
    Hero *hero = getHero(player);
    hero->applyDamageMagic(amount);
}

void GameState::regenMana(double amount, Player player) {
    Hero *hero = getHero(player);
    hero->regenMana(amount);
}

void GameState::spendMana(double amount, Player player) {
    Hero *hero = getHero(player);
    hero->spendMana(amount);
}

bool GameState::canSpendMana(double amount, Player player) const {
    Hero *hero = getHero(player);
    return hero->canSpendMana(amount);
}

bool GameState::isDead(Player player) const {
    Hero *hero = getHero(player);
    return hero->isDead();
}
