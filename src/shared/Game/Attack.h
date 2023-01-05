#pragma once

#include "Unit.h"

class Unit;

class Attack {
public:
    Attack();

    Attack(Unit *attacker, Unit *target);

    void update(double elapsedTimeInSeconds);

    // getters and setters
    [[nodiscard]] bool getMovingFlag() const;
    void setMovingFlag(bool flag);

    [[nodiscard]] int32_t getDamage() const;
    void setDamage(int32_t damage);

    [[nodiscard]] double getMoveSpeed() const;
    void setMoveSpeed(double speed);

    [[nodiscard]] const Point &getPosition() const;
    void setPosition(const Point &position);
    void setPosition(double x, double y);

    [[nodiscard]] Unit *getTarget() const;
    void setTarget(Unit *target);

    [[nodiscard]] Unit *getAttacker() const;
    void setAttacker(Unit *attacker);

private:
    bool moving_;
    Unit *attacker_;
    Unit *target_;
    int32_t damage_;
    double moveSpeed_;
    Position position_;
};

class AttackCreator final {
public:
    explicit AttackCreator(uint32_t attackSpeed);

    Attack *attack(Unit *attacker, std::vector<Unit *> &allUnits);
    Attack *attack(Unit *attacker, Unit *enemy);

    void update(double elapsedTimeInSeconds);

    void refreshCoolDown();

private:
    double coolDown_{0};
    double defaultCoolDown;
};
