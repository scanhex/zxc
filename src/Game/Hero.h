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
    std::vector<Skill> skills_; // needs to be defined in constructor?

public:
    // make everything public or make getters or ?
    int gold_;
    int level_;
    int experience_;

    Hero() = delete;

    Hero(int damage, int attackRange, int moveSpeed, int attackSpeed, int maxHP,
         int maxMP, int hpRegen, int mpRegen, int armor, double resist, Point position);

    void changeGold(int delta);

    void changeLevel(int delta);
    void changeLevelTo(int newLevel);
    void incrLevel();

    void changeExperience(int delta);
};
