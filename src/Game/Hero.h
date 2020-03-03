#pragma once

#include <vector>
#include "Unit.h"
#include "Skill.h"

constexpr int START_GOLD = 239;
constexpr int MAX_LEVEL = 25;
constexpr int EXP_PER_LEVEL = 1000;

class Hero : public Unit {
    // what does a Hero truly need?
private:
    std::vector<Skill> skills_;

    int gold_;
    int level_;
    int experience_;

public:

    Hero() = delete;

    Hero(int damage, int attackRange, int moveSpeed, int attackSpeed, int maxHP,
         int maxMP, int hpRegen, int mpRegen, int armor, double resist, Point position);

    void changeGold(int delta);

    void changeLevel(int delta);
    void incrLevel();
    void changeExperience(int delta);


    // getters and setters

    [[nodiscard]] int getGold() const;
    void setGold(int gold);

    [[nodiscard]] int getLevel() const;
    void setLevel(int level);

    [[nodiscard]] int getExperience() const;
    void setExperience(int experience);
};
