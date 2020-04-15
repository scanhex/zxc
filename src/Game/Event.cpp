#include "Event.h"
#include "GameState.h"

Event::Event() : eventName_{EventName::firstSkill}, player_{Player::First} {}
// microkostyl iz-za boost tsqueue

Event::Event(EventName eventName, Player player) : eventName_{eventName},
                                                   player_{player} {}

Event::Event(EventName eventName, Player player, double x, double y) : eventName_{eventName},
                                                                       player_{player},
                                                                       x_{x},
                                                                       y_{y} {}

uint8_t Event::eventNameToInt() {
    switch (eventName_) {
        case EventName::firstSkill:
            return 1;
        case EventName::secondSkill:
            return 2;
        case EventName::thirdSkill:
            return 3;
        case EventName::move:
            return 4;
        default:
            assert(false);
    }
}

EventName Event::intToEventName(uint8_t eventId) {
    switch (eventId) {
        case 1:
            return EventName::firstSkill;
        case 2:
            return EventName::secondSkill;
        case 3:
            return EventName::thirdSkill;
        case 4:
            return EventName::move;
        default:
            assert(false);
    }
}

