#include "Events.h"

Event::Event() : need_send_{true} {}

MoveEvent::MoveEvent(Hero &hero, double x, double y) : hero_{hero}, x_{x}, y_{y} {}

void MoveEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
    writer.writeDouble(x_);
    writer.writeDouble(y_);
}

void MoveEvent::fire() {
    EventHandler<MoveEvent>::fireEvent(*this);
}


StopEvent::StopEvent(Hero &hero) : hero_{hero} {}

void StopEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
}

void StopEvent::fire() {
    EventHandler<StopEvent>::fireEvent(*this);
}


SkillUseEvent::SkillUseEvent(Hero &hero) : hero_{hero} {}

FirstSkillUseEvent::FirstSkillUseEvent(Hero &hero) : SkillUseEvent(hero) {}
SecondSkillUseEvent::SecondSkillUseEvent(Hero &hero) : SkillUseEvent(hero) {}
ThirdSkillUseEvent::ThirdSkillUseEvent(Hero &hero) : SkillUseEvent(hero) {}

void FirstSkillUseEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
}

void FirstSkillUseEvent::fire() {
    EventHandler<FirstSkillUseEvent>::fireEvent(*this);
}

void SecondSkillUseEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
}

void SecondSkillUseEvent::fire() {
    EventHandler<SecondSkillUseEvent>::fireEvent(*this);
}

void ThirdSkillUseEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
}

void ThirdSkillUseEvent::fire() {
    EventHandler<ThirdSkillUseEvent>::fireEvent(*this);
}
