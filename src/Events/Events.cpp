#include "Events.h"

template<typename T>
void Event::fire(T *event) {
    EventHandler<T>::fireEvent(*event);
}

MoveEvent::MoveEvent(Hero &hero, double x, double y) : hero_{hero}, x_{x}, y_{y} {}

void MoveEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
    writer.writeDouble(x_);
    writer.writeDouble(y_);
}

void MoveEvent::fire() {
    Event::fire(this);
}

StopEvent::StopEvent(Hero &hero) : hero_{hero} {}

void StopEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
}

void StopEvent::fire() {
    Event::fire(this);
}

AttackEvent::AttackEvent(Attack &attack) : attack_{attack} {
}

void AttackEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
    writer.writeUInt8(attack_.getAttacker()->unique_id_);
    writer.writeUInt8(attack_.getTarget()->unique_id_);//poka kostyl'
}

void AttackEvent::fire() {
    Event::fire(this);
}

SkillUseEvent::SkillUseEvent(Hero &hero) : hero_{hero} {}

FirstSkillUseEvent::FirstSkillUseEvent(Hero &hero) : SkillUseEvent(hero) {}

SecondSkillUseEvent::SecondSkillUseEvent(Hero &hero) : SkillUseEvent(hero) {}

ThirdSkillUseEvent::ThirdSkillUseEvent(Hero &hero) : SkillUseEvent(hero) {}

void FirstSkillUseEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
}

void FirstSkillUseEvent::fire() {
    Event::fire(this);
}

void SecondSkillUseEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
}

void SecondSkillUseEvent::fire() {
    Event::fire(this);
}

void ThirdSkillUseEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
}

void ThirdSkillUseEvent::fire() {
    Event::fire(this);
}

void GoldChangedEvent::fire() {
    Event::fire(this);
}

GoldChangedEvent::GoldChangedEvent(int gold) : gold_(gold) {
}

void DrawEvent::fire() {
    Event::fire(this);
}
