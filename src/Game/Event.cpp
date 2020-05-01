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

