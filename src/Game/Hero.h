#pragma once

#include "Unit.h"
#include "Skill.h"
#include "StatsBuilder.h"


enum class SkillNum;
class GameState;
class Skill;


constexpr uint32_t NUM_SKILLS = 3;
constexpr uint32_t START_GOLD = 239;
constexpr uint32_t MAX_LEVEL = 25;
constexpr uint32_t EXP_PER_LEVEL = 1000;

class Hero : public Unit {
    // what does a Hero truly need?
private:
    Player player_;

    uint32_t gold_;
    uint32_t level_;
    uint32_t experience_;

    Skill skills_[NUM_SKILLS];

    static StatsBuilder defaultHeroStatsBuilder;
    static Point firstHeroStartingPoint;
    static Point secondHeroStartingPoint;

public:
    explicit Hero(Player player);
    Hero(Player player, Point position);
    Hero(Player player, Point position, Stats stats);

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
