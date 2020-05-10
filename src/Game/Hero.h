#pragma once

#include "Unit.h"
#include "Skills.h"
#include "Utils/BufferIO.h"
#include "StatsBuilder.h"

enum class Player;


constexpr uint32_t NUM_SKILLS = 3;
constexpr uint32_t START_GOLD = 239;
constexpr uint32_t MAX_LEVEL = 25;
constexpr uint32_t EXP_PER_LEVEL = 1000;

class Hero : public Unit {
    // what does a Hero truly need?
private:

    uint32_t gold_;
    uint32_t level_;
    uint32_t experience_;

    int32_t deathCounter_;

    Coil skills_[NUM_SKILLS];

    static StatsBuilder defaultHeroStatsBuilder_;
    static Position firstHeroStartingPosition_;
    static Position secondHeroStartingPosition_;

public:
    Player player_;

    explicit Hero(Player player);
    Hero(Player player, Position position);
    Hero(Player player, Position position, Stats stats);

    bool canSpendGold(uint32_t amount) const;
    void addGold(uint32_t amount);
    void spendGold(uint32_t amount);

    void levelUp(uint32_t delta);
    void incrementLevel();
    void changeExperience(uint32_t delta);

    bool isSkillReady(SkillName skillName);
    void useSkill(SkillName skillName, GameState &gameState);

    void increaseDeathCounter();

    void updateUnit(double elapsedTimeInSeconds) override;

    void refreshUnit() override;

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
