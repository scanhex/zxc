#include "Events.h"

MoveEvent::MoveEvent(Hero &hero, double x, double y) : hero_{hero}, x_{x}, y_{y} {}

void MoveEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
    writer.writeDouble(x_);
    writer.writeDouble(y_);
}

StopEvent::StopEvent(Hero &hero) : hero_{hero} {}

void StopEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
}


SkillUseEvent::SkillUseEvent(Hero &hero) : hero_{hero} {}


ShortCoilUseEvent::ShortCoilUseEvent(Hero &hero) : SkillUseEvent(hero) {}
MidCoilUseEvent::MidCoilUseEvent(Hero &hero) : SkillUseEvent(hero) {}
LongCoilUseEvent::LongCoilUseEvent(Hero &hero) : SkillUseEvent(hero) {}

void ShortCoilUseEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
}

void MidCoilUseEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
}

void LongCoilUseEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
}
