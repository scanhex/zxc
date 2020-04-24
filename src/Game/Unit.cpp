#include "Unit.h"
#include <algorithm>
#include <cmath>

Unit::Unit(Stats stats, Point position) : stats_{stats},
                                          position_{position},
                                          destination_{position},
                                          angle_{0} {
    stats_.refreshStats();
}


void Unit::addItem(Item &item, size_t slot) {
    if (slot == -1) {
        for (size_t i = 0; i < MAX_ITEMS; i++) {
            if (items_[i].isEmpty()) {
                slot = i;
                break;
            }
        }
        assert(slot != -1 && "No item slots available!");
    } else {
        assert(slot >= 0 && slot < MAX_ITEMS && "Wrong item slot");
        assert(items_[slot].isEmpty() && "Item slot occupied");
    }

    items_[slot] = item;
}

void Unit::deleteItem(size_t indexToDelete) {
    assert(indexToDelete >= 0 && indexToDelete < MAX_ITEMS && "Wrong index for deleting item!");

    items_[indexToDelete] = Item();
}

void Unit::clearItems() {
    for (Item &item : items_) {
        item = Item();
    }
}

void Unit::addBuff(Buff &buff) { buffs_.push_back(buff); }

void Unit::deleteBuff(size_t indexToDelete) {
    std::vector<Buff> newBuffs;
    for (Buff &buff : buffs_) {
        if (buff.index != indexToDelete) {
            newBuffs.push_back(buff);
        }
    }
    std::swap(buffs_, newBuffs);
}

void Unit::clearBuffs() {
    buffs_.clear();
}

void Unit::changeDamage(int32_t delta) { stats_.changeDamage(delta); }
void Unit::changeAttackRange(int32_t delta) { stats_.changeAttackRange(delta); }
void Unit::changeMoveSpeed(int32_t delta) { stats_.changeMoveSpeed(delta); }
void Unit::changeTurnRate(double delta) { stats_.changeTurnRate(delta); }
void Unit::changeAttackSpeed(int32_t delta) { stats_.changeAttackSpeed(delta); }
void Unit::changeMaxHP(int32_t delta) { stats_.changeMaxHP(delta); }
void Unit::changeMaxMP(int32_t delta) { stats_.changeMaxMP(delta); }
void Unit::changeHPRegen(double delta) { stats_.changeHPRegen(delta); }
void Unit::changeMPRegen(double delta) { stats_.changeMPRegen(delta); }
void Unit::changeAngle(double delta) {
    angle_ += delta;
    if (angle_ < 0) angle_ += 2 * M_PI;
    if (angle_ >= 2 * M_PI) angle_ -= 2 * M_PI;
}

void Unit::applyHeal(double amount) {
    stats_.changeHP(amount);
}

void Unit::applyDamage(double amount) {
    stats_.changeHP(-amount);
}

void Unit::applyDamagePhys(double amount) {
    int32_t armor = stats_.getArmor();
    double multiplier = 1 - ((0.052 * armor) / (0.9 + 0.048 * std::abs(armor)));
    applyDamage(amount * multiplier);
}

void Unit::applyDamageMagic(double amount) {
    applyDamage(amount * stats_.getResist());
}

void Unit::regenMana(double amount) {
    stats_.changeMP(amount);
}

bool Unit::canSpendMana(double amount) const {
    return amount >= stats_.getManaPoints();
}

void Unit::spendMana(double amount) {
    assert(canSpendMana(amount) && "Not enough mana points to spend!");
    stats_.changeMP(-amount);
}

void Unit::changeArmor(int32_t delta) { stats_.changeArmor(delta); }
void Unit::changeResist(double delta) { stats_.changeResist(delta); }
void Unit::changePositionBy(double deltaX, double deltaY) { position_ += Point(deltaX, deltaY); }
void Unit::changePositionBy(Point vector) { position_ += vector; }

bool Unit::isDead() { return stats_.getHealthPoints() == 0.0; }


// setters and getters

int32_t Unit::getDamage() const { return stats_.getDamage(); }
void Unit::setDamage(int32_t damage) { stats_.setDamage(damage); }

uint32_t Unit::getAttackRange() const { return stats_.getAttackRange();; }
void Unit::setAttackRange(uint32_t attackRange) { stats_.setAttackRange(attackRange); }

uint32_t Unit::getMoveSpeed() const { return stats_.getMoveSpeed(); }
void Unit::setMoveSpeed(uint32_t moveSpeed) { stats_.setMoveSpeed(moveSpeed); }

double Unit::getTurnRate() const { return stats_.getTurnRate(); }
void Unit::setTurnRate(double turnRate) { stats_.setTurnRate(turnRate); }

uint32_t Unit::getAttackSpeed() const { return stats_.getAttackSpeed(); }
void Unit::setAttackSpeed(uint32_t attackSpeed) { stats_.setAttackSpeed(attackSpeed); }

uint32_t Unit::getMaxHp() const { return stats_.getMaxHp(); }
void Unit::setMaxHp(uint32_t maxHp) { stats_.setMaxHp(maxHp); }

uint32_t Unit::getMaxMp() const { return stats_.getMaxMp(); }
void Unit::setMaxMp(uint32_t maxMp) { stats_.setMaxMp(maxMp); }

double Unit::getHealthPoints() const { return stats_.getHealthPoints(); }
void Unit::setHealthPoints(double healthPoints) { stats_.setHealthPoints(healthPoints); }

double Unit::getManaPoints() const { return stats_.getManaPoints(); }
void Unit::setManaPoints(double manaPoints) { stats_.setManaPoints(manaPoints); }

double Unit::getHpRegen() const { return stats_.getHpRegen(); }
void Unit::setHpRegen(double hpRegen) { stats_.setHpRegen(hpRegen); }

double Unit::getMpRegen() const { return stats_.getMpRegen(); }
void Unit::setMpRegen(double mpRegen) { stats_.setMpRegen(mpRegen); }

int32_t Unit::getArmor() const { return stats_.getArmor(); }
void Unit::setArmor(int32_t armor) { stats_.setArmor(armor); }

double Unit::getResist() const { return stats_.getResist(); }
void Unit::setResist(double resist) { stats_.setResist(resist); }

const Point &Unit::getPosition() const { return position_; }
void Unit::setPosition(const Point &position) { position_ = position; }
void Unit::setPosition(double x, double y) { position_ = Point(x, y); }

const Point &Unit::getDestination() const { return destination_; }
void Unit::setDestination(const Point &destination) { destination_ = destination; }
void Unit::setDestination(double x, double y) { destination_ = Point(x, y); }

void Unit::setAngle(double angle) { angle_ = angle; }
double Unit::getAngle() const { return angle_; }
