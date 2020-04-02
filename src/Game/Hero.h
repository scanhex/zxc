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
    Hero(Stats stats, Point position);

    bool canSpendGold(unsigned amount);
    void addGold(unsigned amount);
    void spendGold(unsigned amount);

    void levelUp(unsigned delta);
    void incrementLevel();
    void changeExperience(unsigned delta);


    // getters and setters

    [[nodiscard]] unsigned getGold() const;
    void setGold(unsigned gold);

    [[nodiscard]] unsigned getLevel() const;
    void setLevel(unsigned level);

    [[nodiscard]] unsigned getExperience() const;
    void setExperience(unsigned experience);
};
