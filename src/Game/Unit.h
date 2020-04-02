#pragma once

#include <vector>
#include <cassert>

#include "Stats.h"
#include "Point.h"
#include "Item.h"
#include "Buff.h"

constexpr size_t MAX_ITEMS = 6;

class Unit {
private:
    Item items_[MAX_ITEMS];
    std::vector<Buff> buffs_;

    Stats stats_;

    Point position_; // ??

public:
    Unit() = delete;
    Unit(Stats stats, Point position);

    void addItem(Item &item, int slot = -1);

    void deleteItem(int indexToDelete); // 0..MAX_ITEMS-1 in inventory
    void clearItems();

    void addBuff(Buff &buff);

    void deleteBuff(int indexToDelete); // deletes 1 buff matching buff.index
    void clearBuffs();

    void changeDamage(int delta);
    void changeAttackRange(int delta);
    void changeMoveSpeed(int delta);
    void changeAttackSpeed(int delta);
    void changeMaxHP(int delta);
    void changeMaxMP(int delta);
    void changeHPRegen(double delta);
    void changeMPRegen(double delta);

    void heal(double amount);
    void damage(double amount);
    void damagePhys(double amount);
    void damageMagic(double amount);

    void regenMana(double amount);
    void spendMana(double amount);
    [[nodiscard]] bool canSpendMana(double amount) const;

    void changeArmor(int delta);
    void changeResist(double delta);
    void changePosition(double deltaX, double deltaY); // coords += deltaCoords

    bool isDead();


    // getters and setters

    [[nodiscard]] int getDamage() const;
    void setDamage(int damage);

    [[nodiscard]] unsigned getAttackRange() const;
    void setAttackRange(unsigned attackRange);

    [[nodiscard]] unsigned getMoveSpeed() const;
    void setMoveSpeed(unsigned moveSpeed);

    [[nodiscard]] unsigned getAttackSpeed() const;
    void setAttackSpeed(unsigned attackSpeed);

    [[nodiscard]] unsigned getMaxHp() const;
    void setMaxHp(unsigned maxHp);

    [[nodiscard]] unsigned getMaxMp() const;
    void setMaxMp(unsigned maxMp);

    [[nodiscard]] double getHealthPoints() const;
    void setHealthPoints(double healthPoints);

    [[nodiscard]] double getManaPoints() const;
    void setManaPoints(double manaPoints);

    [[nodiscard]] double getHpRegen() const;
    void setHpRegen(double hpRegen);

    [[nodiscard]] double getMpRegen() const;
    void setMpRegen(double mpRegen);

    [[nodiscard]] int getArmor() const;
    void setArmor(int armor);

    [[nodiscard]] double getResist() const;
    void setResist(double resist);

    [[nodiscard]] const Point &getPosition() const;
    void setPosition(const Point &position);
    void setPosition(double x, double y);
};
