#pragma once

#include <vector>
#include "Unit.h"
#include "Skill.h"

enum class Player;
enum class SkillNum;
class GameState;
class Skill;

constexpr uint32_t START_GOLD = 239;
constexpr uint32_t MAX_LEVEL = 25;
constexpr uint32_t EXP_PER_LEVEL = 1000;

class Hero : public Unit {
    // what does a Hero truly need?
private:
    Player player_;

    std::vector<Skill> skills_;

    uint32_t gold_;
    uint32_t level_;
    uint32_t experience_;

public:
    Hero(Stats stats, Point position, Player player);

    bool canSpendGold(uint32_t amount);
    void addGold(uint32_t amount);
    void spendGold(uint32_t amount);

    void levelUp(uint32_t delta);
    void incrementLevel();
    void changeExperience(uint32_t delta);

    void useSkill(SkillNum skillNum, GameState &gameState);

    // getters and setters

    [[nodiscard]] uint32_t getGold() const;
    void setGold(uint32_t gold);

    [[nodiscard]] uint32_t getLevel() const;
    void setLevel(uint32_t level);

    [[nodiscard]] uint32_t getExperience() const;
    void setExperience(uint32_t experience);
};
