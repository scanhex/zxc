#include "Events.h"

std::vector<MoveEventHandler *> MoveEventHandler::allHandlers;
std::vector<SkillUseEventHandler *> SkillUseEventHandler::allHandlers;

MoveEvent::MoveEvent(Hero &hero, double x, double y) : hero_{hero}, x_{x}, y_{y} {}

void MoveEvent::serialize(uint8_t (&buffer)[1024]) {
    BufferIO::writeUInt8(static_cast<uint8_t>(name_), 0, buffer);
    BufferIO::writeDouble(x_, 1, buffer);
    BufferIO::writeDouble(y_, 9, buffer);
}

SkillUseEvent::SkillUseEvent(Hero &hero) : hero_{hero} {}

void SkillUseEvent::serialize(uint8_t (&buffer)[1024]) {
    BufferIO::writeUInt8(static_cast<uint8_t>(name_), 0, buffer);
}
