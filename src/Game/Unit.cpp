#define _USE_MATH_DEFINES

#include "Unit.h"
#include "Utils/Utils.h"
#include <algorithm>
#include <cmath>

uint8_t Unit::radiant_counter_ = 0;
uint8_t Unit::dire_counter = 255;

Unit::Unit(Stats stats, Position position) : team_{Team::Neutral},
                                             goldKillReward_{0},
                                             expKillReward_{0},
                                             stats_{stats},
                                             position_{position},
                                             heroRadius_{0.24}{
    stats_.refreshStats();
    creator_ = new AttackCreator();
}


void Unit::addItem(Item &item, size_t slot) {
    // TODO add items??
    items_[slot] = item;
}

void Unit::deleteItem(size_t indexToDelete) {
    // TODO add items??
    items_[indexToDelete] = Item();
}

void Unit::clearItems() {
    for (Item &item : items_) {
        item = Item();
    }
}

void Unit::addBuff(Buff &buff) {
    // TODO add buffs??
    buffs_.push_back(buff);
}

void Unit::deleteBuff(size_t indexToDelete) {
    // TODO add buffs??
    unused_parameter(indexToDelete);
}

void Unit::clearBuffs() {
    buffs_.clear();
}

Attack* Unit::attack(std::vector<Unit * >& allUnits) {
    return creator_->attack(this, allUnits);
}

void Unit::changeDamage(int32_t delta) { stats_.changeDamage(delta); }
void Unit::changeAttackRange(int32_t delta) { stats_.changeAttackRange(delta); }
void Unit::changeMoveSpeed(int32_t delta) { stats_.changeMoveSpeed(delta); }
void Unit::changeTurnRate(double delta) { stats_.changeTurnRate(delta); }
void Unit::changeAttackSpeed(int32_t delta) { stats_.changeAttackSpeed(delta); }
void Unit::changeMaxHP(int32_t delta) { stats_.changeMaxHP(delta); }
void Unit::changeMaxMP(int32_t delta) { stats_.changeMaxMP(delta); }
void Unit::changeHPRegen(double delta) { stats_.changeHPRegen(delta); }
void Unit::changeMPRegen(double delta) { stats_.changeMPRegen(delta); }

void Unit::applyHeal(double amount) {
    stats_.changeHP(amount);
}

void Unit::applyDamage(double amount) {
    stats_.changeHP(-amount);
}

void Unit::applyDamagePhys(double amount) {
    int32_t armor = stats_.getArmor();
    double multiplier = 1 - ((0.052 * armor) / (0.9 + 0.048 * std::abs(armor)));
    applyDamage(amount * multiplier);
}

void Unit::applyDamageMagic(double amount) {
    applyDamage(amount * stats_.getResist());
}

void Unit::regenMana(double amount) {
    stats_.changeMP(amount);
}

bool Unit::canSpendMana(double amount) const {
    return amount >= stats_.getManaPoints();
}

void Unit::spendMana(double amount) {
    assert(canSpendMana(amount) && "Not enough mana points to spend!");
    stats_.changeMP(-amount);
}

void Unit::changeArmor(int32_t delta) { stats_.changeArmor(delta); }
void Unit::changeResist(double delta) { stats_.changeResist(delta); }

void Unit::updateUnit(double elapsedTimeInSeconds, std::vector<Unit * >& allUnits) {
    if (isDead()) return;
    creator_->update(elapsedTimeInSeconds);
    applyHeal(getHpRegen() * elapsedTimeInSeconds);
    regenMana(getMpRegen() * elapsedTimeInSeconds);

    double turnDelta = getTurnRate() * (elapsedTimeInSeconds / 0.03);
    double moveDelta = (getMoveSpeed() / 100.0) * elapsedTimeInSeconds;
    position_.updateAngle(turnDelta);
    Point nextPos = position_.nextPosition(moveDelta);
    if (checkUnitsPosition(nextPos, allUnits)) {
        //  position_.update(turnDelta, moveDelta);
        moved_ = true;
        position_.updatePoint(moveDelta);
    } else {
        moved_ = false;
    }
}

void Unit::claimReward(Unit *killed_unit) {
    return;
}

bool Unit::checkUnitsPosition(const Point& position, std::vector<Unit * >& allUnits) const {
    for (auto & unit : allUnits) {
        if (unit != this) {
            if (!Point::isEnoughDistance(position, heroRadius_,
                                         unit->getPosition(), unit->heroRadius_)) {
                return false;
            }
        }
    }
    return true;
}

void Unit::refreshUnit() {
    creator_->refreshCoolDown();
    stats_.refreshStats();
}

void Unit::serialize(BufferIO::BufferWriter &writer) {
    // items, buffs?
    stats_.serialize(writer);
    position_.serialize(writer);
}

void Unit::deserialize(BufferIO::BufferReader &reader) {
    // items, buffs?
    stats_.deserialize(reader);
    position_.deserialize(reader);
}

Point Unit::shiftUnitPosition(double len) const {
    double angle = position_.getAngle();
    double xShift = std::abs(sin(angle) * len);
    double yShift = std::abs(cos(angle) * len);
    if (angle < M_PI) xShift = -xShift;
    if (angle > M_PI / 2 && angle < 3 * M_PI / 2) yShift = -yShift;
    return position_.getPosition() + Point(xShift, yShift);
}

bool Unit::inRadius(double x, double y, double r) const {
    return position_.inRadius(x, y, r);
}

bool Unit::inRadius(const Point &point, double r) const {
    return position_.inRadius(point, r);
}

bool Unit::isDead() const { return stats_.getHealthPoints() == 0.0; }


// setters and getters
double Unit::getHeroRadius() const { return heroRadius_; }
Team Unit::getTeam() const { return team_; }
uint32_t Unit::getGoldKillReward() const { return goldKillReward_; }
uint32_t Unit::getExpKillReward() const { return expKillReward_; }

bool Unit::getMovedFlag() const { return moved_; }
void Unit::setMovedFlag(bool status) { moved_ = status; }

int32_t Unit::getDamage() const { return stats_.getDamage(); }
void Unit::setDamage(int32_t damage) { stats_.setDamage(damage); }

uint32_t Unit::getAttackRange() const { return stats_.getAttackRange();; }
void Unit::setAttackRange(uint32_t attackRange) { stats_.setAttackRange(attackRange); }

uint32_t Unit::getMoveSpeed() const { return stats_.getMoveSpeed(); }
void Unit::setMoveSpeed(uint32_t moveSpeed) { stats_.setMoveSpeed(moveSpeed); }

double Unit::getTurnRate() const { return stats_.getTurnRate(); }
void Unit::setTurnRate(double turnRate) { stats_.setTurnRate(turnRate); }

uint32_t Unit::getAttackSpeed() const { return stats_.getAttackSpeed(); }
void Unit::setAttackSpeed(uint32_t attackSpeed) { stats_.setAttackSpeed(attackSpeed); }

uint32_t Unit::getMaxHp() const { return stats_.getMaxHp(); }
void Unit::setMaxHp(uint32_t maxHp) { stats_.setMaxHp(maxHp); }

uint32_t Unit::getMaxMp() const { return stats_.getMaxMp(); }
void Unit::setMaxMp(uint32_t maxMp) { stats_.setMaxMp(maxMp); }

double Unit::getHealthPoints() const { return stats_.getHealthPoints(); }
void Unit::setHealthPoints(double healthPoints) { stats_.setHealthPoints(healthPoints); }

double Unit::getManaPoints() const { return stats_.getManaPoints(); }
void Unit::setManaPoints(double manaPoints) { stats_.setManaPoints(manaPoints); }

double Unit::getHpRegen() const { return stats_.getHpRegen(); }
void Unit::setHpRegen(double hpRegen) { stats_.setHpRegen(hpRegen); }

double Unit::getMpRegen() const { return stats_.getMpRegen(); }
void Unit::setMpRegen(double mpRegen) { stats_.setMpRegen(mpRegen); }

int32_t Unit::getArmor() const { return stats_.getArmor(); }
void Unit::setArmor(int32_t armor) { stats_.setArmor(armor); }

double Unit::getResist() const { return stats_.getResist(); }
void Unit::setResist(double resist) { stats_.setResist(resist); }

const Point &Unit::getPosition() const { return position_.getPosition(); }
void Unit::setPosition(const Point &position) { position_.setPosition(position); }
void Unit::setPosition(double x, double y) { position_.setPosition(x, y); }

const Point &Unit::getDestination() const { return position_.getDestination(); }
void Unit::setDestination(const Point &destination) { position_.setDestination(destination); }
void Unit::setDestination(double x, double y) { position_.setDestination(x, y); }

double Unit::getAngle() const { return position_.getAngle(); }
void Unit::setAngle(double angle) {
    position_.setAngle(angle);
}

void Unit::giveId() {
    if(team_ == Team::Neutral)
        return; //TODO
    if(team_ == Team::Dire)
            unique_id_ = dire_counter--;
    if(team_ == Team::Radiant)
        unique_id_ = radiant_counter_++;
}
