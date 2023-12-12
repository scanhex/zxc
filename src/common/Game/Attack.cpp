#include "Attack.h"

#include <algorithm>

Attack::Attack()
    : moving_{false},
      attacker_{nullptr},
      target_{nullptr},
      damage_{0},
      moveSpeed_{1200},
      position_{Point(1000, 1000), 0} {}

Attack::Attack(Unit *attacker, Unit *target)
    : moving_{true},
      attacker_{attacker},
      target_{target},
      damage_{attacker->getDamage()},
      moveSpeed_{1200},
      position_{attacker->getPosition(), 0} {
    position_.setDestination(target->getPosition());
}

void Attack::update(double elapsedTimeInSeconds) {
    if (target_ && target_->isDead()) {
        moving_ = false;
    }
    if (!moving_) {
        return;
    }
    position_.setDestination(target_->getPosition());
    double moveDelta = (moveSpeed_ / 100.0) * elapsedTimeInSeconds;
    position_.updatePointIgnoreAngle(moveDelta);
    if (Point::getDistance(target_->getPosition(), position_.getPosition()) <= target_->getHeroRadius()) {
        moving_ = false;
        position_.setPosition({1000, 1000});
        position_.setDestination({1000, 1000});
        target_->applyDamage(damage_);
        if (target_->isDead()) {
            attacker_->claimReward(target_);
        }
    }
}

bool Attack::getMovingFlag() const {
    return moving_;
}
void Attack::setMovingFlag(bool flag) {
    moving_ = flag;
}

int32_t Attack::getDamage() const {
    return damage_;
}
void Attack::setDamage(int32_t damage) {
    damage_ = damage;
}

double Attack::getMoveSpeed() const {
    return moveSpeed_;
}
void Attack::setMoveSpeed(double speed) {
    moveSpeed_ = speed;
}

const Point &Attack::getPosition() const {
    return position_.getPosition();
}
void Attack::setPosition(const Point &position) {
    position_.setPosition(position);
}
void Attack::setPosition(double x, double y) {
    position_.setPosition(x, y);
}

Unit *Attack::getTarget() const {
    return target_;
}
void Attack::setTarget(Unit *target) {
    target_ = target;
}

Unit *Attack::getAttacker() const {
    return attacker_;
}
void Attack::setAttacker(Unit *attacker) {
    attacker_ = attacker;
    damage_ = attacker->getDamage();
}

AttackCreator::AttackCreator(uint32_t attackSpeed) : defaultCoolDown{100.0 / attackSpeed} {}

Attack *AttackCreator::attack(Unit *attacker, std::vector<Unit *> &allUnits) {
    if (coolDown_ != 0) {
        return nullptr;
    }
    Unit *enemy = nullptr;
    double dist = attacker->getAttackRange();
    for (Unit *unit : allUnits) {
        if (unit->getTeam() != attacker->getTeam() && !unit->isDead()) {
            double this_dist = Point::getDistance(attacker->getPosition(), unit->getPosition());
            if (dist > this_dist) {
                enemy = unit;
                dist = this_dist;
            }
        }
    }
    if (!enemy) {
        return nullptr;
    }
    auto *attack = new Attack(attacker, enemy);  // TODO better way to choose target
    coolDown_ = defaultCoolDown;
    return attack;
}

Attack *AttackCreator::attack(Unit *attacker, Unit *enemy) {
    if (coolDown_ != 0 || !enemy) {
        return nullptr;
    }
    auto *attack = new Attack(attacker, enemy);  // TODO better way to choose target
    coolDown_ = defaultCoolDown;
    return attack;
}

void AttackCreator::update(double elapsedTimeInSeconds) {
    coolDown_ = std::max(0.0, coolDown_ - elapsedTimeInSeconds);
}

void AttackCreator::refreshCoolDown() {
    coolDown_ = 0.0;
}
