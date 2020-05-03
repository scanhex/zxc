#pragma once

#include "Events/Events.h"

class GameState;


class GameStateHandler : public EventHandler<MoveEvent>,
                         public EventHandler<ShortCoilUseEvent>,
                         public EventHandler<MidCoilUseEvent>,
                         public EventHandler<LongCoilUseEvent> {
public:
    explicit GameStateHandler(GameState &gameState);

    void handle(MoveEvent &event) override;
    void handle(ShortCoilUseEvent &event) override;
    void handle(MidCoilUseEvent &event) override;
    void handle(LongCoilUseEvent &event) override;

private:
    GameState &gameState_;
};
