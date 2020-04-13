#include "Skill.h"

Skill::Skill(Player player, SkillNum type) : Event{player}, type_{type} {}

void Skill::handleEvent(GameState &gs) {
    double damage = 0;
    switch (type_) {
        case SkillNum::first:
            damage = 10;
            break;
        case SkillNum::second:
            damage = 20;
            break;
        case SkillNum::third:
            damage = 30;
            break;
        default:
            assert(false);
    }
    // for now just damages other player
    if (player_ == Player::First) gs.applyDamage(damage, Player::Second);
    else gs.applyDamage(damage, Player::First);
}
