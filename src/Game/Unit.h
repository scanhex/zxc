#pragma once

#include <vector>
#include <cassert>

#include "Point.h"
#include "Item.h"
#include "Buff.h"

constexpr int MAX_ITEMS = 6;

class Unit {
private:
    Item items_[MAX_ITEMS];
    std::vector<Buff> buffs_;

    int damage_;
    int attackRange_;

    int moveSpeed_;
    int attackSpeed_;

    int maxHP_;
    int maxMP_;
    double healthPoints_;
    double manaPoints_;
    double hpRegen_;
    double mpRegen_;

    int armor_; // adjust formula(see damagePhys)?
    double resist_; // percent

    Point position_; // ??

public:
    Unit() = delete;

    Unit(int damage, int attackRange, int moveSpeed, int attackSpeed, int maxHP, int maxMP, double hpRegen,
         double mpRegen, int armor, double resist, Point position);

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
    void changeHPRegen(int delta);
    void changeMPRegen(int delta);

    void heal(double amount);
    void damage(double amount);
    void damagePhys(double amount);
    void damageMagic(double amount);

    void regenMana(int amount);
    void spendMana(int amount);
    bool canSpendMana(int amount);

    void changeArmor(int delta);
    void changeResist(int delta);
    void changePosition(double deltaX, double deltaY); // coords += deltaCoords

    bool isDead();


    // getters and setters

    [[nodiscard]] int getDamage() const;
    void setDamage(int damage);

    [[nodiscard]] int getAttackRange() const;
    void setAttackRange(int attackRange);

    [[nodiscard]] int getMoveSpeed() const;
    void setMoveSpeed(int moveSpeed);

    [[nodiscard]] int getAttackSpeed() const;
    void setAttackSpeed(int attackSpeed);

    [[nodiscard]] int getMaxHp() const;
    void setMaxHp(int maxHp);

    [[nodiscard]] int getMaxMp() const;
    void setMaxMp(int maxMp);

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
