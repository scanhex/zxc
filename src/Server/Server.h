#pragma once

#ifdef _MSC_VER
#include <algorithm>
#endif

#include "Game/GameState.h"
#include "Utils/BufferIO.h"
#include <cstdio>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/lockfree/queue.hpp>

static constexpr int MAX_MSG = 1024;
static constexpr int TICK_TIME_GS_UPDATE = 10;
static constexpr int TICK_TIME_SEND_GS = 30;
static constexpr int PLAYERS_REQUIRED = 2;
static constexpr int MSG_FROM_SERVER_SIZE = 512;
static constexpr int MSG_FROM_CLIENT_SIZE = 32;
static constexpr int MSG_WAIT_FROM_SERVER_SIZE = 8;

using namespace boost::asio;

#define BIND_FN(x)         std::bind(&ConnectionToClient::x, shared_from_this())
#define BIND_FN1(x, y)      std::bind(&ConnectionToClient ::x, shared_from_this(),y)
#define BIND_FN2(x, y, z)    std::bind(&ConnectionToClient ::x, shared_from_this(),y,z)
#define BIND_FN3(x, y, z, w)  std::bind(&ConnectionToClient ::x, shared_from_this(),y,z,w)

class Server final {
public:
    Server();

    void run();

private:

    class ConnectionToClient : public std::enable_shared_from_this<ConnectionToClient> {

    public:

        ConnectionToClient(io_service &service, GameState &gs, bool &running, bool &stopped, ip::tcp::acceptor &ac,
                           std::mutex &lock, boost::lockfree::queue<Event *> &myEvents,
                           boost::lockfree::queue<Event *> &othersEvents);

        void startConnection();

        static int32_t connectionsNumber();

        void stopConnection();

        ip::tcp::socket &sock();

    private:
        /*
         * Functions for handling connection to client, e.g. reading/writing to socket
         */
        void waitForAllConnections(const boost::system::error_code &err, size_t bytes);

        void handleReadFromSocket(const boost::system::error_code &err, size_t bytes);

        void readFromSocket();

        size_t checkReadComplete(const boost::system::error_code &err, size_t bytes);

        void handleWriteToSocket(const boost::system::error_code &err, size_t bytes);

        void writeToSocket();

        void sendEndGameMessage();

    private:
        /*
         * Functions for working with Game State
         */
        void updateGSbyPlayer();

        void writeGStoBuffer();

        void writeEventsToBuffer();

    private:

        void connectionChecker();

    public:

        void startChecker();

    private:
        GameState &gameState_;
        bool &running_;
        bool &stopped_;
        ip::tcp::acceptor &acceptor_;
        std::mutex &g_lock_;
        static int32_t running_connections_;
        bool is_connected_{false};
        int player_id_{};
        ip::tcp::socket sock_;
        BufferIO::BufferReader reader_{};
        BufferIO::BufferWriter writer_{};
        deadline_timer timer_;
        deadline_timer stop_timer_;
        boost::lockfree::queue<Event *> &myEvents_;
        boost::lockfree::queue<Event *> &othersEvents_;

    public:
        std::thread conn_checker_;
    };

private:

    void updateGS();

    void handleNewConnection(const std::shared_ptr<ConnectionToClient> &client, const boost::system::error_code &err);

    void runGameStateCycle();

    std::shared_ptr<ConnectionToClient> newClient();

private:

    GameState gameState{};
    bool running_{false};
    bool stopped_{false};
    boost::posix_time::ptime last_tick{};
    boost::posix_time::ptime now{};
    boost::lockfree::queue<Event *> firstPlayerEvents_{20};
    boost::lockfree::queue<Event *> secondPlayerEvents_{20};
    io_service service_{};
    ip::tcp::acceptor acceptor_{service_, ip::tcp::endpoint(ip::tcp::v4(), 8080)};
    std::mutex gs_lock_{};
};

void runServer();
