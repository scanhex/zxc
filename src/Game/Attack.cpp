#include "Attack.h"

Attack::Attack(Unit *attacker, Unit *target) : target_{target},
                                               damage_{attacker->getDamage()},
                                               moveSpeed_{600},
                                               position_{attacker->getPosition(), 0} {
    position_.setDestination(target->getPosition());
}

void Attack::update(double elapsedTimeInSeconds) {
    if (!moving_)
        return;
    position_.setDestination(target_->getDestination());
    double moveDelta = (moveSpeed_ / 100.0) * elapsedTimeInSeconds;
    position_.updatePointIgnoreAngle(moveDelta);
    if (Point::getDistance(target_->getPosition(), position_.getPosition()) <= target_->getHeroRadius() * 2) { //TODO *2?
        moving_ = false;
        target_->applyDamage(damage_);
    }
}

bool Attack::getMovingFlag() const { return moving_; }
void Attack::setMovingFlag(bool flag) { moving_ = flag; }

int32_t Attack::getDamage() const { return damage_; }
void Attack::setDamage(int32_t damage) {damage_ = damage; }

double Attack::getMoveSpeed() const { return moveSpeed_; }
void Attack::setMoveSpeed(double speed) { moveSpeed_ = speed; }

const Point &Attack::getPosition() const { return position_.getPosition(); }
void Attack::setPosition(const Point &position) { position_.setPosition(position); }
void Attack::setPosition(double x, double y) { position_.setPosition(x, y); }

Unit *Attack::getTarget() const { return target_;}
void Attack::setTarget(Unit *target) { target_ = target; }
