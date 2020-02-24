#include "Unit.h"

Unit::Unit(int damage, int attackRange, int moveSpeed, int attackSpeed, int maxHP,
           int maxMP, int hpRegen, int mpRegen, int armor, double resist, Point position) :
        damage_{damage}, attackRange_{attackRange}, moveSpeed_{moveSpeed},
        attackSpeed_{attackSpeed}, maxHP_{maxHP}, maxMP_{maxMP},
        healthPoints_{maxHP}, manaPoints_{maxMP},
        hpRegen_{hpRegen}, mpRegen_{mpRegen}, armor_{armor},
        resist_{resist}, position_{position} {}


void Unit::addItem(Item item) {
    assert(items_.size() < MAX_ITEMS && "Too much items!");

    items_.push_back(item);
}

void Unit::deleteItem(int indexToDelete) {
    assert(indexToDelete >= 0 && indexToDelete < items_.size() && "Wrong index for deleting item!");

    items_.erase(items_.begin() + indexToDelete);
}

void Unit::addBuff(Buff buff) { buffs_.push_back(buff); }

void Unit::deleteBuff(int indexToDelete) {
    std::vector<Buff> newBuffs;
    for (Buff &buff : buffs_) {
        if (buff.index != indexToDelete) {
            newBuffs.push_back(buff);
        }
    }
    std::swap(buffs_, newBuffs);
}

void Unit::changeDamage(int delta) { damage_ += delta; }

void Unit::changeAttackRange(int delta) { attackRange_ += delta; }

void Unit::changeMoveSpeed(int delta) { moveSpeed_ += delta; }

void Unit::changeAttackSpeed(int delta) { attackSpeed_ += delta; }

void Unit::changeHPRegen(int delta) { hpRegen_ += delta; }

void Unit::changeMPRegen(int delta) { mpRegen_ += delta; }

void Unit::heal(int amount) {
    healthPoints_ = std::min(healthPoints_ + amount, maxHP_);
}

void Unit::damage(int amount) {
    healthPoints_ = std::max(healthPoints_ - amount, 0);
}

void Unit::damagePhys(int amount) {
    double multiplier = 1 - ((0.052 * armor_) / (0.9 + 0.048 * std::abs(armor_)));
    damage((int) (amount * multiplier));
}

void Unit::damageMagic(int amount) {
    damage((int) (amount * resist_));
}

void Unit::regenMana(int amount) {
    manaPoints_ = std::min(manaPoints_ += amount, maxMP_);
}

bool Unit::canSpendMana(int amount) {
    return amount >= manaPoints_;
}

void Unit::spendMana(int amount) {
    assert(amount >= manaPoints_ && "Not enough mana points to spend!");

    manaPoints_ -= amount;
}

void Unit::changeArmor(int delta) { armor_ += delta; }

void Unit::changeResist(int delta) { resist_ += delta; }

void Unit::changePosition(double deltaX, double deltaY) {
    position_ += Point(deltaX, deltaY);
}

void Unit::changePositionTo(double x, double y) {
    position_ = Point(x, y);
}

bool Unit::isDead() { return healthPoints_ <= 0; }
