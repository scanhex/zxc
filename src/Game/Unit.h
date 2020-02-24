#ifndef ZXC_UNIT_H
#define ZXC_UNIT_H

#include <vector>
#include "Point.h"
#include "Item.h"
#include "Buff.h"

constexpr int MAX_ITEMS = 6;

class Unit{
private:
    std::vector<Item> items_; // changeable
    std::vector<Buff> buffs_; // changeable

    // make everything public or make getters or ?

    int damage_;
    int attackRange_;

    int moveSpeed_;
    int attackSpeed_;

    int maxHP_;
    int maxMP_;
    int healthPoints_;
    int manaPoints_;
    int hpRegen_; // per sec, make double?
    int mpRegen_; // per sec, make double?

    int armor_; // adjust formula(see damagePhys)?
    double resist_; // percent

    Point position_; // ??

public:
    Unit() = delete;

    Unit(int damage, int attackRange, int moveSpeed, int attackSpeed, int maxHP, int maxMP, int hpRegen, int mpRegen, int armor, double resist, Point position);

    void addItem(Item item);
    void deleteItem(int indexToDelete); // 0..MAX_ITEMS-1 in inventory

    void addBuff(Buff buff);
    void deleteBuff(int indexToDelete); // deletes 1 buff matching buff.index

    void changeDamage(int delta);
    void changeAttackRange(int delta);

    void changeMoveSpeed(int delta);
    void changeAttackSpeed(int delta);

    void changeHPRegen(int delta);
    void changeMPRegen(int delta);

    void damage(int amount);
    void heal(int amount);

    void damagePhys(int amount);
    void damageMagic(int amount);

    void regenMana(int amount);
    void spendMana(int amount);
    bool canSpendMana(int amount);

    void changeArmor(int delta);
    void changeResist(int delta);

    void changePosition(double deltaX, double deltaY); // coords += deltaCoords
    void changePositionTo(double x, double y); // coords = {x, y}

    bool isDead();
};

#endif //ZXC_UNIT_H
