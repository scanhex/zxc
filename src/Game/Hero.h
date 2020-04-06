#pragma once

#include <vector>
#include "Unit.h"
#include "Skill.h"


constexpr uint32_t SKILLS_COUNT = 3;
constexpr uint32_t START_GOLD = 239;
constexpr uint32_t MAX_LEVEL = 25;
constexpr uint32_t EXP_PER_LEVEL = 1000;

class Hero : public Unit {
    // what does a Hero truly need?
private:
    Skill skills_[SKILLS_COUNT];

    uint32_t gold_;
    uint32_t level_;
    uint32_t experience_;

public:
    Hero(Stats stats, Point position);

    bool canSpendGold(uint32_t amount);
    void addGold(uint32_t amount);
    void spendGold(uint32_t amount);

    void levelUp(uint32_t delta);
    void incrementLevel();
    void changeExperience(uint32_t delta);


    // getters and setters

    [[nodiscard]] uint32_t getGold() const;
    void setGold(uint32_t gold);

    [[nodiscard]] uint32_t getLevel() const;
    void setLevel(uint32_t level);

    [[nodiscard]] uint32_t getExperience() const;
    void setExperience(uint32_t experience);
};
