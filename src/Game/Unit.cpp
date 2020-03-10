#include "Unit.h"
#include <algorithm>

Unit::Unit(int damage, unsigned attackRange, unsigned moveSpeed, unsigned attackSpeed, unsigned maxHP,
           unsigned maxMP, double hpRegen, double mpRegen, int armor, double resist, Point position) :
        damage_{damage}, attackRange_{attackRange}, moveSpeed_{moveSpeed},
        attackSpeed_{attackSpeed}, maxHP_{maxHP}, maxMP_{maxMP},
        healthPoints_{static_cast<double>(maxHP)}, manaPoints_{static_cast<double>(maxMP)},
        hpRegen_{hpRegen}, mpRegen_{mpRegen}, armor_{armor},
        resist_{resist}, position_{position} {}


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

void Unit::changeDamage(int delta) { damage_ += delta; }

void Unit::changeAttackRange(int delta) { attackRange_ += delta; }

void Unit::changeMoveSpeed(int delta) { moveSpeed_ += delta; }

void Unit::changeAttackSpeed(int delta) { attackSpeed_ += delta; }

void Unit::changeHPRegen(int delta) { hpRegen_ += delta; }

void Unit::changeMPRegen(int delta) { mpRegen_ += delta; }

void Unit::heal(double amount) {
    healthPoints_ = std::min(healthPoints_ + amount, static_cast<double>(maxHP_));
}

void Unit::damage(double amount) {
    healthPoints_ = std::max(healthPoints_ - amount, 0.0);
}

void Unit::damagePhys(double amount) {
    double multiplier = 1 - ((0.052 * armor_) / (0.9 + 0.048 * std::abs(armor_)));
    damage(amount * multiplier);
}

void Unit::damageMagic(double amount) {
    damage(amount * resist_);
}

void Unit::regenMana(unsigned amount) {
    manaPoints_ += amount;
    manaPoints_ = std::min(manaPoints_, static_cast<double>(maxMP_));
}

bool Unit::canSpendMana(unsigned amount) {
    return amount >= manaPoints_;
}

void Unit::spendMana(unsigned amount) {
    assert(amount >= manaPoints_ && "Not enough mana points to spend!");

    manaPoints_ -= amount;
}

void Unit::changeArmor(int delta) { armor_ += delta; }

void Unit::changeResist(int delta) { resist_ += delta; }

void Unit::changePosition(double deltaX, double deltaY) { position_ += Point(deltaX, deltaY); }

bool Unit::isDead() { return healthPoints_ <= 0; }


// setters and getters

int Unit::getDamage() const { return damage_; }

void Unit::setDamage(int damage) { damage_ = damage; } // forbid negative damage?

unsigned Unit::getAttackRange() const { return attackRange_; }

void Unit::setAttackRange(unsigned attackRange) {
    attackRange_ = attackRange;
}

unsigned Unit::getMoveSpeed() const { return moveSpeed_; }

void Unit::setMoveSpeed(unsigned moveSpeed) {
    moveSpeed_ = moveSpeed;
}

unsigned Unit::getAttackSpeed() const { return attackSpeed_; }

void Unit::setAttackSpeed(unsigned attackSpeed) {
    attackSpeed_ = attackSpeed;
}

unsigned Unit::getMaxHp() const { return maxHP_; }

void Unit::setMaxHp(unsigned maxHp) {
    maxHP_ = maxHp;
}

unsigned Unit::getMaxMp() const { return maxMP_; }

void Unit::setMaxMp(unsigned maxMp) {
    maxMP_ = maxMp;
}

double Unit::getHealthPoints() const { return healthPoints_; }

void Unit::setHealthPoints(double healthPoints) {
    assert(healthPoints >= 0 && healthPoints <= maxHP_ && "Wrong new health points!");
    healthPoints_ = healthPoints;
}

double Unit::getManaPoints() const { return manaPoints_; }

void Unit::setManaPoints(double manaPoints) {
    assert(manaPoints >= 0 && manaPoints <= maxHP_ && "Wrong new mana points!");
    manaPoints_ = manaPoints;
}

double Unit::getHpRegen() const { return hpRegen_; }

void Unit::setHpRegen(double hpRegen) { hpRegen_ = hpRegen; }

double Unit::getMpRegen() const { return mpRegen_; }

void Unit::setMpRegen(double mpRegen) { mpRegen_ = mpRegen; }

int Unit::getArmor() const { return armor_; }

void Unit::setArmor(int armor) { armor_ = armor; }

double Unit::getResist() const { return resist_; }

void Unit::setResist(double resist) {
    assert(resist >= 0 && resist <= 1 && "Wrong new resist!");
    resist_ = resist;
}

const Point &Unit::getPosition() const { return position_; }

void Unit::setPosition(const Point &position) { position_ = position; }

void Unit::setPosition(double x, double y) { position_ = Point(x, y); }
