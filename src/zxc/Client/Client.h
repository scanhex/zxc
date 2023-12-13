#pragma once

#include "Events/Events.h"
#include "Game/GameState.h"
#include "Utils/BufferIO.h"

#include <boost/asio.hpp>
#include <boost/lockfree/queue.hpp>

#include <iostream>
#include <string>
#include <thread>
#include <utility>

static constexpr int MAX_MSG = 1024;
static constexpr int MSG_FROM_SERVER_SIZE = 512;
static constexpr int MSG_FROM_CLIENT_SIZE = 32;  // TODO change when add
static constexpr int MSG_WAIT_FROM_SERVER_SIZE = 8;

using namespace boost::asio;

#define BIND_FN(x) std::bind(&ConnectionToServer ::x, shared_from_this())
#define BIND_FN1(x, y) std::bind(&ConnectionToServer ::x, shared_from_this(), y)
#define BIND_FN2(x, y, z) std::bind(&ConnectionToServer ::x, shared_from_this(), y, z)
#define BIND_FN3(x, y, z, w) std::bind(&ConnectionToServer ::x, shared_from_this(), y, z, w)

class Client final : EventHandler<MoveEvent>,
                     EventHandler<StopEvent>,
                     EventHandler<AttackEvent>,
                     EventHandler<FirstSkillUseEvent>,
                     EventHandler<SecondSkillUseEvent>,
                     EventHandler<ThirdSkillUseEvent>,
                     EventHandler<DrawEvent> {
public:
    explicit Client(GameState &gameState);

    void run();

private:
    class ConnectionToServer : public std::enable_shared_from_this<ConnectionToServer> {
    public:
        explicit ConnectionToServer(GameState &gameState);

        static std::shared_ptr<ConnectionToServer> newConnection(GameState &gameState);

        void startConnection();

        void stopConnection();

        bool isConnected() const;

        void fireOtherEvents();

    private:
        /*
         * Functions for handling connection to client, e.g. reading/writing to socket
         */
        void handleConnection(const boost::system::error_code &err);

        void handleWriteToSocket(const boost::system::error_code &err, size_t bytes);

        void writeToSocket();

        void handleReadFromSocket(const boost::system::error_code &err, size_t bytes);

        void readFromSocket();

        size_t checkReadComplete(const boost::system::error_code &err, size_t bytes);

        size_t checkWaitReadComplete(const boost::system::error_code &err, size_t bytes);

        void waitForAction();

        void waitForGameStart();

        void handleWaitRead(const boost::system::error_code &err, size_t bytes);

        void runGame();

    private:
        /*
         * Functions for working with local Game State and player's actions
         */
        void parseGSFromBuffer();

        void parseEventsFromBuffer();

        void writeActionToBuffer();

        void clearEvents();

    public:
        void runService();

    private:
        io_service service_;
        ip::tcp::endpoint ep_;
        // 127.0.0.1 to test locally
        // 51.250.68.234 artem minecraft
        ip::tcp::socket sock_;
        BufferIO::BufferReader reader_{};
        BufferIO::BufferWriter writer_{};
        bool connected_{};
        deadline_timer timer_;
        deadline_timer stop_timer_;
        GameState &gameState_;
        Player current_ = Player::First;

    public:
        boost::lockfree::queue<SerializedEvent *> othersEvents_{100};
        boost::lockfree::queue<SerializedEvent *> events_{100};
    };

private:
    void handle(const MoveEvent &event) override;

    void handle(const StopEvent &event) override;

    void handle(const AttackEvent &event) override;

    void handle(const FirstSkillUseEvent &event) override;

    void handle(const SecondSkillUseEvent &event) override;

    void handle(const ThirdSkillUseEvent &event) override;

    void handle(const DrawEvent &event) override;

private:
    std::shared_ptr<ConnectionToServer> connection_;
    GameState &gameState_;

private:
    template<typename E, typename = std::enable_if_t<std::is_base_of_v<Event, E>>>
    static bool isNotFromServerEvent(E &e);

    class FromServerEvent {};

    class FromServerMoveEvent : public MoveEvent, public FromServerEvent {
    public:
        explicit FromServerMoveEvent(Hero &hero, double x, double y) : MoveEvent(hero, x, y) {}
    };

    class FromServerStopEvent : public StopEvent, public FromServerEvent {
    public:
        explicit FromServerStopEvent(Hero &hero) : StopEvent(hero) {}
    };

    class FromServerAttackEvent : public AttackEvent, public FromServerEvent {
    public:
        explicit FromServerAttackEvent(uint8_t attackerID, uint8_t targetID) : AttackEvent(attackerID, targetID) {}
    };

    class FromServerFirstSkillUseEvent : public FirstSkillUseEvent, public FromServerEvent {
    public:
        explicit FromServerFirstSkillUseEvent(Hero &hero) : FirstSkillUseEvent(hero) {}
    };

    class FromServerSecondSkillUseEvent : public SecondSkillUseEvent, public FromServerEvent {
    public:
        explicit FromServerSecondSkillUseEvent(Hero &hero) : SecondSkillUseEvent(hero) {}
    };

    class FromServerThirdSkillUseEvent : public ThirdSkillUseEvent, public FromServerEvent {
    public:
        explicit FromServerThirdSkillUseEvent(Hero &hero) : ThirdSkillUseEvent(hero) {}
    };
};