#include "GameState.h"

GameState::GameState(Hero &firstHero, Hero &secondHero) : firstHero_{&firstHero},
                                                          secondHero_{&secondHero} {}

GameState::GameState() {
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
    firstHero_ = new Hero(heroStatsBuilder.create(), Point(0, 0), Player::First);
    secondHero_ = new Hero(heroStatsBuilder.create(), Point(0, 0), Player::Second);
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

bool GameState::gameIsFinished() {
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

void GameState::applyMove(Player player, int32_t x, int32_t y) {
    // ----
}

void GameState::applyEvent(Player player, EventName eventName) {
    Hero *hero = firstHero_;
    if (player == Player::Second) hero = secondHero_;

    switch (eventName) {
        case EventName::firstSkill:
            hero->useSkill(SkillNum::first, *this);
            break;
        case EventName::secondSkill:
            hero->useSkill(SkillNum::second, *this);
            break;
        case EventName::thirdSkill:
            hero->useSkill(SkillNum::third, *this);
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
