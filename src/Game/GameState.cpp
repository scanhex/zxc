#include "GameState.h"

GameState::GameState(Hero &firstHero, Hero &secondHero) : firstHero_{&firstHero},
                                                          secondHero_{&secondHero} {}

GameState::GameState() {
    // 2 default heros
    firstHero_ = new Hero(Point(0, 0), Player::First);
    secondHero_ = new Hero(Point(0, 0), Player::Second);
}

double GameState::getHealthPoints(Player player) {
    switch (player) {
        case Player::First:
            return firstHero_->getHealthPoints();
        case Player::Second:
            return secondHero_->getHealthPoints();
        default:
            assert(false);
    }
}

Point GameState::getPosition(Player player) {
    switch (player) {
        case Player::First:
            return firstHero_->getPosition();
        case Player::Second:
            return secondHero_->getPosition();
        default:
            assert(false);
    }
}

void GameState::setPosition(Point pos, Player player) {
    switch (player) {
        case Player::First:
            firstHero_->setPosition(pos);
            break;
        case Player::Second:
            secondHero_->setPosition(pos);
            break;
        default:
            assert(false);
    }
}

void GameState::setPosition(double x, double y, Player player) {
    switch (player) {
        case Player::First:
            firstHero_->setPosition(x, y);
            break;
        case Player::Second:
            secondHero_->setPosition(x, y);
            break;
        default:
            assert(false);
    }
}

void GameState::setHealthPoints(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero_->setHealthPoints(amount);
            break;
        case Player::Second:
            secondHero_->setHealthPoints(amount);
            break;
        default:
            assert(false);
    }
}

bool GameState::gameIsFinished() const {
    return firstHero_->isDead() || secondHero_->isDead();
}

void GameState::update(double elapsedTime) {
    // only regen for now
    double healPerTick = firstHero_->getHpRegen() * elapsedTime;
    double manaPerTick = firstHero_->getMpRegen() * elapsedTime;
    firstHero_->applyHeal(healPerTick);
    firstHero_->regenMana(manaPerTick);

    healPerTick = secondHero_->getHpRegen() * elapsedTime;
    manaPerTick = secondHero_->getMpRegen() * elapsedTime;
    secondHero_->applyHeal(healPerTick);
    secondHero_->regenMana(manaPerTick);
}

void GameState::applyMove(Player player, double x, double y) {
    // TODO blink -> move
    switch (player) {
        case Player::First:
            firstHero_->setPosition(x, y);
            break;
        case Player::Second:
            secondHero_->setPosition(x, y);
            break;
        default:
            assert(false);
    }
}

void GameState::applyEvent(Event event) {
    Hero *hero = firstHero_;
    if (event.player_ == Player::Second) hero = secondHero_;

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
    switch (player) {
        case Player::First:
            firstHero_->applyDamage(amount);
            break;
        case Player::Second:
            secondHero_->applyDamage(amount);
            break;
        default:
            assert(false);
    }
}

void GameState::applyDamagePhys(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero_->applyDamagePhys(amount);
            break;
        case Player::Second:
            secondHero_->applyDamagePhys(amount);
            break;
        default:
            assert(false);
    }
}

void GameState::applyDamageMagic(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero_->applyDamageMagic(amount);
            break;
        case Player::Second:
            secondHero_->applyDamageMagic(amount);
            break;
        default:
            assert(false);
    }
}

void GameState::regenMana(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero_->regenMana(amount);
            break;
        case Player::Second:
            secondHero_->regenMana(amount);
            break;
        default:
            assert(false);
    }
}

void GameState::spendMana(double amount, Player player) {
    switch (player) {
        case Player::First:
            firstHero_->spendMana(amount);
            break;
        case Player::Second:
            secondHero_->spendMana(amount);
            break;
        default:
            assert(false);
    }
}

bool GameState::canSpendMana(double amount, Player player) const {
    switch (player) {
        case Player::First:
            return firstHero_->canSpendMana(amount);
        case Player::Second:
            return secondHero_->canSpendMana(amount);
        default:
            assert(false);
    }
}

bool GameState::isDead(Player player) const {
    switch (player) {
        case Player::First:
            return firstHero_->isDead();
        case Player::Second:
            return secondHero_->isDead();
        default:
            assert(false);
    }
}
