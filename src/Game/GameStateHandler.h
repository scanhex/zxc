#pragma once

#include "Events/Events.h"

class GameState;


class GameStateHandler : public EventHandler<MoveEvent>,
                         public EventHandler<ShortCoilUseEvent>,
                         public EventHandler<MidCoilUseEvent>,
                         public EventHandler<LongCoilUseEvent> {
public:
    explicit GameStateHandler(GameState &gameState);

    void handle(const MoveEvent &event) override;
    void handle(const ShortCoilUseEvent &event) override;
    void handle(const MidCoilUseEvent &event) override;
    void handle(const LongCoilUseEvent &event) override;

private:
    GameState &gameState_;
};
