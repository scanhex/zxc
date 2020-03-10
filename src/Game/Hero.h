#pragma once

#include <vector>
#include "Unit.h"
#include "Skill.h"

constexpr unsigned START_GOLD = 239;
constexpr unsigned MAX_LEVEL = 25;
constexpr unsigned EXP_PER_LEVEL = 1000;

class Hero : public Unit {
    // what does a Hero truly need?
private:
    std::vector<Skill> skills_;

    unsigned gold_;
    unsigned level_;
    unsigned experience_;

public:

    Hero() = delete;

    Hero(int damage, unsigned attackRange, unsigned moveSpeed, unsigned attackSpeed, unsigned maxHP,
         unsigned maxMP, double hpRegen, double mpRegen, int armor, double resist, Point position);

    void changeGold(int delta);

    void levelUp(unsigned delta);
    void incrementLevel();
    void getExperience(unsigned delta);


    // getters and setters

    [[nodiscard]] unsigned getGold() const;
    void setGold(unsigned gold);

    [[nodiscard]] unsigned getLevel() const;
    void setLevel(unsigned level);

    [[nodiscard]] unsigned getExperience() const;
    void setExperience(unsigned experience);
};
