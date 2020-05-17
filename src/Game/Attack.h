#pragma once
#include <Game/Unit.h>

class Attack {
public:
    Attack(Unit* attacker, Unit* target);
    void update(double elapsedTimeInSeconds);

    //getters and setters
    [[nodiscard]] bool getMovingFlag() const;
    void setMovingFlag(bool flag);

    [[nodiscard]] int32_t getDamage() const;
    void setDamage(int32_t damage);

    [[nodiscard]] double getMoveSpeed() const;
    void setMoveSpeed(double speed);

    [[nodiscard]] const Point &getPosition() const;
    void setPosition(const Point &position);
    void setPosition(double x, double y);

    [[nodiscard]] Unit* getTarget() const;
    void setTarget(Unit * target);

private:
    bool moving_{true};
    Unit* target_;
    int32_t damage_;
    double moveSpeed_;
    Position position_;
};
