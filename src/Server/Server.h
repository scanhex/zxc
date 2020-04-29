#pragma once

#ifdef _MSC_VER
#include <algorithm>
#endif

#include <cstdio>
#include <iostream>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>

static constexpr int MAX_MSG = 1024;
static constexpr int TICK_TIME_GS_UPDATE = 10;
static constexpr int TICK_TIME_SEND_GS = 10;
static constexpr int PLAYERS_REQUIRED = 2;
static constexpr int MSG_FROM_SERVER_SIZE = 128;
static constexpr int MSG_FROM_CLIENT_SIZE = 32; //TODO change when add move action
static constexpr int MSG_WAIT_FROM_SERVER_SIZE = 8;

using namespace boost::asio;

#define BIND_FN(x)         std::bind(&ConnectionToClient::x, shared_from_this())
#define BIND_FN1(x, y)      std::bind(&ConnectionToClient ::x, shared_from_this(),y)
#define BIND_FN2(x, y, z)    std::bind(&ConnectionToClient ::x, shared_from_this(),y,z)
#define BIND_FN3(x, y, z, w)  std::bind(&ConnectionToClient ::x, shared_from_this(),y,z,w)


class ConnectionToClient : public std::enable_shared_from_this<ConnectionToClient> {

public:

    ConnectionToClient();

    void startConnection();

    static std::shared_ptr<ConnectionToClient> newClient();

    int32_t connectionsNumber() const;

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

private:
    /*
     * Functions for working with Game State
     */
    void updateGSbyPlayer();

    void writeGStoBuffer();

private:

    void connectionChecker();

public:

    void startChecker();

private:
    static int32_t running_connections_;
    bool is_connected_{false};
    int player_id_{};
    ip::tcp::socket sock_;
    uint8_t read_buffer_[MAX_MSG]{};
    uint8_t write_buffer_[MAX_MSG]{};
    deadline_timer timer_;
    deadline_timer stop_timer_;

public:
    std::thread conn_checker_;
};

void updateGS();

void handleNewConnection(const std::shared_ptr<ConnectionToClient> &client, const boost::system::error_code &err);

void runGameStateCycle();

void runServer();
