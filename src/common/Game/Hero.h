#pragma once

#include "Skills.h"
#include "StatsBuilder.h"
#include "Unit.h"
#include "Utils/BufferIO.h"

enum class Player : uint8_t;

constexpr uint32_t NUM_PLAYERS = 2;
constexpr uint32_t NUM_SKILLS = 3;
constexpr uint32_t START_GOLD = 239;
constexpr uint32_t MAX_LEVEL = 25;
constexpr uint32_t EXP_PER_LEVEL = 1000;
constexpr double HERO_RESPAWN_TIME = 1;

class Hero : public Unit {
    // what does a Hero truly need?
private:
    uint32_t gold_;
    uint32_t level_;
    uint32_t experience_;

    uint32_t hpIncreasePerLevel_;
    uint32_t mpIncreasePerLevel_;
    int32_t damageIncreasePerLevel_;

    uint32_t deathCounter_;

    Coil skills_[NUM_SKILLS];

    static StatsBuilder defaultHeroStatsBuilder_;
    static Position heroSpawns_[NUM_PLAYERS];

public:
    explicit Hero(Player player);
    Hero(Player player, Position position);
    Hero(Player player, Position position, Stats stats);

    bool isHero() override;

    [[nodiscard]] bool canSpendGold(uint32_t amount) const;
    void addGold(uint32_t amount);
    void spendGold(uint32_t amount);

    void levelUp(uint32_t delta);
    void incrementLevel();
    void changeExperience(uint32_t delta);

    void claimReward(Unit *killed_unit) override;

    bool isSkillReady(SkillName skillName);
    void useSkill(SkillName skillName, GameState &gameState);

    void updateUnit(double elapsedTimeInSeconds, std::vector<Unit *> &allUnits) override;

    void refreshPosition();
    void refreshUnit() override;

    void updateStats();
    void updateDamage();
    void updateMaxHP();
    void updateMaxMP();

    void serialize(BufferIO::BufferWriter &writer) override;
    void deserialize(BufferIO::BufferReader &reader) override;

    // getters and setters

    [[nodiscard]] uint32_t getGold() const;
    void setGold(uint32_t gold);

    [[nodiscard]] uint32_t getLevel() const;
    void setLevel(uint32_t level);

    [[nodiscard]] uint32_t getExperience() const;
    void setExperience(uint32_t experience);

    [[nodiscard]] uint32_t getDeathCounter() const;
    void setDeathCounter(uint32_t number);
};
