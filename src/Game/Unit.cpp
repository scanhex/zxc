#include "Unit.h"
#include <algorithm>

Unit::Unit(Stats stats, Point position) : stats_{stats},
                                          position_{position} {
    stats_.refreshStats();
}


void Unit::addItem(Item &item, int slot) {
    if (slot == -1) {
        for (int i = 0; i < MAX_ITEMS; i++) {
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

void Unit::deleteItem(int indexToDelete) {
    assert(indexToDelete >= 0 && indexToDelete < MAX_ITEMS && "Wrong index for deleting item!");

    items_[indexToDelete] = Item();
}

void Unit::clearItems() {
    for (auto &item : items_) {
        item = Item();
    }
}

void Unit::addBuff(Buff &buff) { buffs_.push_back(buff); }

void Unit::deleteBuff(int indexToDelete) {
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

void Unit::changeDamage(int delta) { stats_.changeDamage(delta); }
void Unit::changeAttackRange(int delta) { stats_.changeAttackRange(delta); }
void Unit::changeMoveSpeed(int delta) { stats_.changeMoveSpeed(delta); }
void Unit::changeAttackSpeed(int delta) { stats_.changeAttackSpeed(delta); }
void Unit::changeMaxHP(int delta) { stats_.changeMaxHP(delta); }
void Unit::changeMaxMP(int delta) { stats_.changeMaxMP(delta); }
void Unit::changeHPRegen(double delta) { stats_.changeHPRegen(delta); }
void Unit::changeMPRegen(double delta) { stats_.changeMPRegen(delta); }

void Unit::heal(double amount) {
    stats_.changeHP(amount);
}

void Unit::damage(double amount) {
    stats_.changeHP(-amount);
}

void Unit::damagePhys(double amount) {
    int armor = stats_.getArmor();
    double multiplier = 1 - ((0.052 * armor) / (0.9 + 0.048 * std::abs(armor)));
    damage(amount * multiplier);
}

void Unit::damageMagic(double amount) {
    damage(amount * stats_.getResist());
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

void Unit::changeArmor(int delta) { stats_.changeArmor(delta); }
void Unit::changeResist(double delta) { stats_.changeResist(delta); }
void Unit::changePosition(double deltaX, double deltaY) { position_ += Point(deltaX, deltaY); }

bool Unit::isDead() { return stats_.getHealthPoints() == 0.0; }


// setters and getters

int Unit::getDamage() const { return stats_.getDamage(); }
void Unit::setDamage(int damage) { stats_.setDamage(damage); }

unsigned Unit::getAttackRange() const { return stats_.getAttackRange();; }
void Unit::setAttackRange(unsigned attackRange) { stats_.setAttackRange(attackRange); }

unsigned Unit::getMoveSpeed() const { return stats_.getMoveSpeed(); }
void Unit::setMoveSpeed(unsigned moveSpeed) { stats_.setMoveSpeed(moveSpeed); }

unsigned Unit::getAttackSpeed() const { return stats_.getAttackSpeed(); }
void Unit::setAttackSpeed(unsigned attackSpeed) { stats_.setAttackSpeed(attackSpeed); }

unsigned Unit::getMaxHp() const { return stats_.getMaxHp(); }
void Unit::setMaxHp(unsigned maxHp) { stats_.setMaxHp(maxHp); }

unsigned Unit::getMaxMp() const { return stats_.getMaxMp(); }
void Unit::setMaxMp(unsigned maxMp) { stats_.setMaxMp(maxMp); }

double Unit::getHealthPoints() const { return stats_.getHealthPoints();; }
void Unit::setHealthPoints(double healthPoints) { stats_.setManaPoints(healthPoints); }

double Unit::getManaPoints() const { return stats_.getManaPoints(); }
void Unit::setManaPoints(double manaPoints) { stats_.setManaPoints(manaPoints); }

double Unit::getHpRegen() const { return stats_.getHpRegen(); }
void Unit::setHpRegen(double hpRegen) { stats_.setMpRegen(hpRegen); }

double Unit::getMpRegen() const { return stats_.getMpRegen(); }
void Unit::setMpRegen(double mpRegen) { stats_.setMpRegen(mpRegen); }

int Unit::getArmor() const { return stats_.getArmor(); }
void Unit::setArmor(int armor) { stats_.setArmor(armor); }

double Unit::getResist() const { return stats_.getResist(); }
void Unit::setResist(double resist) { stats_.setResist(resist); }

const Point &Unit::getPosition() const { return position_; }
void Unit::setPosition(const Point &position) { position_ = position; }
void Unit::setPosition(double x, double y) { position_ = Point(x, y); }
