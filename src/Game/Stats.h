#pragma once

class Stats {
private:
    int damage_;
    unsigned attackRange_;

    unsigned moveSpeed_;
    unsigned attackSpeed_;

    unsigned maxHP_;
    unsigned maxMP_;
    double healthPoints_;
    double manaPoints_;
    double hpRegen_;
    double mpRegen_;

    int armor_; // adjust formula(see damagePhys)?
    double resist_; // percent
public:
    Stats() = default;
    Stats(const Stats &) = default;

    void changeDamage(int delta);
    void changeAttackRange(int delta);
    void changeMoveSpeed(int delta);
    void changeAttackSpeed(int delta);
    void changeMaxHP(int delta);
    void changeMaxMP(int delta);
    void changeHP(double delta);
    void changeMP(double delta);
    void changeHPRegen(double delta);
    void changeMPRegen(double delta);
    void changeArmor(int delta);
    void changeResist(double delta);

    void refreshStats();

    // getters and setters

    [[nodiscard]] int getDamage() const;
    void setDamage(int damage);

    [[nodiscard]] unsigned getAttackRange() const;
    void setAttackRange(unsigned attackRange);

    [[nodiscard]] unsigned getMoveSpeed() const;
    void setMoveSpeed(unsigned moveSpeed);

    [[nodiscard]] unsigned getAttackSpeed() const;
    void setAttackSpeed(unsigned attackSpeed);

    [[nodiscard]] unsigned getMaxHp() const;
    void setMaxHp(unsigned maxHp);

    [[nodiscard]] unsigned getMaxMp() const;
    void setMaxMp(unsigned maxMp);

    [[nodiscard]] double getHealthPoints() const;
    void setHealthPoints(double healthPoints);

    [[nodiscard]] double getManaPoints() const;
    void setManaPoints(double manaPoints);

    [[nodiscard]] double getHpRegen() const;
    void setHpRegen(double hpRegen);

    [[nodiscard]] double getMpRegen() const;
    void setMpRegen(double mpRegen);

    [[nodiscard]] int getArmor() const;
    void setArmor(int armor);

    [[nodiscard]] double getResist() const;
    void setResist(double resist);
};
