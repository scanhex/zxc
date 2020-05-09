#pragma once

#include <cstdint>

class GameState;
class Hero;

enum class SkillName : uint8_t {
    FirstSkill = 0,
    SecondSkill = 1,
    ThirdSkill = 2
};

class Skill {
public:
    explicit Skill(Hero &hero);
    virtual ~Skill() = default;
    virtual void use(GameState &gameState) = 0;

protected:
    Hero &hero_;
};

class Coil : public Skill {
public:
    constexpr static double COOL_DOWN = 2;

    Coil(Hero &hero, double len, double radius, double damage);

    void use(GameState &gameState) override;
    virtual void update(double elapsedTimeInSeconds);
    void refreshCoolDown();
    [[nodiscard]] bool isReady() const;

protected:
    double coolDown_;
    double len_, radius_, damage_;
};

class ShortCoil final : public Coil {
public:
    explicit ShortCoil(Hero &hero);

    constexpr static double LEN = 2;
    constexpr static double RADIUS = 1.5;
    constexpr static double DAMAGE = 100;
};

class MidCoil final : public Coil {
public:
    explicit MidCoil(Hero &hero);

    constexpr static double LEN = 4.5;
    constexpr static double RADIUS = 1.5;
    constexpr static double DAMAGE = 200;
};

class LongCoil final : public Coil {
public:
    explicit LongCoil(Hero &hero);

    constexpr static double LEN = 7;
    constexpr static double RADIUS = 1.5;
    constexpr static double DAMAGE = 300;
};

