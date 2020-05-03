#include "Events.h"

template<> std::vector<EventHandler<MoveEvent> *> EventHandler<MoveEvent>::allHandlers{};
template<> std::vector<EventHandler<ShortCoilUseEvent> *> EventHandler<ShortCoilUseEvent>::allHandlers{};
template<> std::vector<EventHandler<MidCoilUseEvent> *> EventHandler<MidCoilUseEvent>::allHandlers{};
template<> std::vector<EventHandler<LongCoilUseEvent> *> EventHandler<LongCoilUseEvent>::allHandlers{};


MoveEvent::MoveEvent(Hero &hero, double x, double y) : hero_{hero}, x_{x}, y_{y} {}

void MoveEvent::serialize(BufferIO::BufferWriter &writer) {
    writer.writeUInt8(static_cast<uint8_t>(name_));
    writer.writeDouble(x_);
    writer.writeDouble(y_);
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
