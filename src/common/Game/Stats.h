#pragma once

#include "Utils/BufferIO.h"

#include <cstdint>

class Stats {
private:
    int32_t damage_;
    int32_t default_damage_;
    double attackRange_;

    uint32_t moveSpeed_;
    uint32_t attackSpeed_;
    double turnRate_;

    uint32_t maxHP_;
    uint32_t maxMP_;
    uint32_t default_maxHP_;
    uint32_t default_maxMP_;
    double healthPoints_;
    double manaPoints_;
    double hpRegen_;  // per second
    double mpRegen_;  // per second

    int32_t armor_;  // adjust formula(see applyDamagePhys)?
    double resist_;  // percent
public:
    Stats() = default;
    Stats(const Stats &) = default;
    Stats &operator=(const Stats &) = default;

    void changeDamage(int32_t delta);
    void changeAttackRange(double delta);
    void changeMoveSpeed(int32_t delta);
    void changeTurnRate(int32_t delta);
    void changeAttackSpeed(int32_t delta);
    void changeMaxHP(int32_t delta);
    void changeMaxMP(int32_t delta);
    void changeHP(double delta);
    void changeMP(double delta);
    void changeHPRegen(double delta);
    void changeMPRegen(double delta);
    void changeArmor(int32_t delta);
    void changeResist(double delta);

    void refreshStats();

    void serialize(BufferIO::BufferWriter &writer) const;
    void deserialize(BufferIO::BufferReader &reader);

    // getters and setters

    [[nodiscard]] int32_t getDamage() const;
    void setDamage(int32_t damage);

    [[nodiscard]] int32_t getDefaultDamage() const;
    void setDefaultDamage(int32_t damage);

    [[nodiscard]] double getAttackRange() const;
    void setAttackRange(double attackRange);

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

    [[nodiscard]] uint32_t getDefaultHealthPoints() const;
    void setDefaultHealthPoints(uint32_t healthPoints);

    [[nodiscard]] double getManaPoints() const;
    void setManaPoints(double manaPoints);

    [[nodiscard]] uint32_t getDefaultManaPoints() const;
    void setDefaultManaPoints(uint32_t manaPoints);

    [[nodiscard]] double getHpRegen() const;
    void setHpRegen(double hpRegen);

    [[nodiscard]] double getMpRegen() const;
    void setMpRegen(double mpRegen);

    [[nodiscard]] int32_t getArmor() const;
    void setArmor(int32_t armor);

    [[nodiscard]] double getResist() const;
    void setResist(double resist);
};
