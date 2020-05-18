#pragma once

#include <vector>
#include <cassert>

#include "Stats.h"
#include "Point.h"
#include "Item.h"
#include "Buff.h"
#include "Position.h"
#include "Attack.h"
#include "Utils/BufferIO.h"

class AttackCreator;
class Attack;

enum class Team : uint8_t {
    Radiant = 0,
    Dire = 1,
    Neutral = 2
};

constexpr size_t MAX_ITEMS = 6;
constexpr uint32_t NUM_TEAMS = 3;

class Unit {
public:
    uint8_t unique_id_;

protected:
    Item items_[MAX_ITEMS];
    std::vector<Buff> buffs_;

    Team team_;
    uint32_t goldKillReward_;
    uint32_t expKillReward_;

    Stats stats_;

    Position position_;
    double heroRadius_;
    bool moved_{false};

    AttackCreator* creator_;

    static uint8_t radiant_counter_;
    static uint8_t dire_counter;

public:
    Unit(Stats stats, Position position);

    void addItem(Item &item, size_t slot = -1);

    void deleteItem(size_t indexToDelete); // 0..MAX_ITEMS-1 in inventory TODO
    void clearItems();

    void addBuff(Buff &buff);

    void deleteBuff(size_t indexToDelete); // deletes 1 buff matching buff.index TODO
    void clearBuffs();

    Attack* attack(std::vector<Unit * >& allUnits);

    void giveId();

    void changeDamage(int32_t delta);
    void changeAttackRange(int32_t delta);
    void changeMoveSpeed(int32_t delta);
    void changeTurnRate(double delta);
    void changeAttackSpeed(int32_t delta);
    void changeMaxHP(int32_t delta);
    void changeMaxMP(int32_t delta);
    void changeHPRegen(double delta);
    void changeMPRegen(double delta);

    void applyHeal(double amount);
    void applyDamage(double amount);
    void applyDamagePhys(double amount);
    void applyDamageMagic(double amount);

    void regenMana(double amount);
    void spendMana(double amount);
    [[nodiscard]] bool canSpendMana(double amount) const;

    void changeArmor(int32_t delta);
    void changeResist(double delta);

    virtual void updateUnit(double elapsedTimeInSeconds, std::vector<Unit * >& allUnits);

    virtual void claimReward(Unit* killed_unit);

    virtual void refreshUnit();

    [[nodiscard]] bool checkUnitsPosition(const Point& position, std::vector<Unit * >& allUnits) const;

    virtual void serialize(BufferIO::BufferWriter &writer);
    virtual void deserialize(BufferIO::BufferReader &reader);

    [[nodiscard]] Point shiftUnitPosition(double len) const;
    [[nodiscard]] bool inRadius(double x, double y, double r) const;
    [[nodiscard]] bool inRadius(const Point &point, double r) const;
    [[nodiscard]] bool isDead() const;


    // getters and setters

    [[nodiscard]] Team getTeam() const;
    [[nodiscard]] uint32_t getGoldKillReward() const;
    [[nodiscard]] uint32_t getExpKillReward() const;
    [[nodiscard]] double getHeroRadius() const;

    [[nodiscard]] bool getMovedFlag() const;
    void setMovedFlag(bool status);

    [[nodiscard]] int32_t getDamage() const;
    void setDamage(int32_t damage);

    [[nodiscard]] uint32_t getAttackRange() const;
    void setAttackRange(uint32_t attackRange);

    [[nodiscard]] uint32_t getMoveSpeed() const;
    void setMoveSpeed(uint32_t moveSpeed);

    [[nodiscard]] double getTurnRate() const;
    void setTurnRate(double turnRate);

    [[nodiscard]] uint32_t getAttackSpeed() const;
    void setAttackSpeed(uint32_t attackSpeed);

    [[nodiscard]] uint32_t getMaxHp() const;
    void setMaxHp(uint32_t maxHp);

    [[nodiscard]] uint32_t getMaxMp() const;
    void setMaxMp(uint32_t maxMp);

    [[nodiscard]] double getHealthPoints() const;
    void setHealthPoints(double healthPoints);

    [[nodiscard]] double getManaPoints() const;
    void setManaPoints(double manaPoints);

    [[nodiscard]] double getHpRegen() const;
    void setHpRegen(double hpRegen);

    [[nodiscard]] double getMpRegen() const;
    void setMpRegen(double mpRegen);

    [[nodiscard]] int32_t getArmor() const;
    void setArmor(int32_t armor);

    [[nodiscard]] double getResist() const;
    void setResist(double resist);

    [[nodiscard]] const Point &getPosition() const;
    void setPosition(const Point &position);
    void setPosition(double x, double y);

    [[nodiscard]] const Point &getDestination() const;
    void setDestination(const Point &destination);
    void setDestination(double x, double y);

    [[nodiscard]] double getAngle() const;
    void setAngle(double angle);
};
