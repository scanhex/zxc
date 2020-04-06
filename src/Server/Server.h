#ifndef SERVER_H
#define SERVER_H

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
static constexpr int MSG_FROM_SERVER_SIZE = 16;//size of default message to client
static constexpr int MSG_FROM_CLIENT_SIZE = 8;

using namespace boost::asio;

#define BIND_FN(x)         std::bind(&self_type::x, shared_from_this())
#define BIND_FN1(x, y)      std::bind(&self_type::x, shared_from_this(),y)
#define BIND_FN2(x, y, z)    std::bind(&self_type::x, shared_from_this(),y,z)
#define BIND_FN3(x, y, z, w)  std::bind(&self_type::x, shared_from_this(),y,z,w)



class ConnectionToClient : public std::enable_shared_from_this<ConnectionToClient> {

public:
    typedef std::shared_ptr<ConnectionToClient> ptr;
    typedef ConnectionToClient self_type;

    ConnectionToClient();

    void startConnection();

    static ptr newClient();

    int connectionsNumber() const;

    void stopConnection();

    ip::tcp::socket &sock();

private:
    /*
     * Functions for handling connection to client, e.g. reading/writing to socket
     */
    void waitForAllConnections(const boost::system::error_code &err);

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
    /*
     *  Functions to parse args to bytes and back
     */
    void writeDouble(double d,int start_idx);
    void writeInt32(int32_t d,int start_idx);
    void writeInt64(int64_t d,int start_idx);

    double readDouble(int start_idx);
    int32_t readInt32(int start_idx);
    int64_t readInt64(int start_idx);
    union binaryDouble {
        double dValue;
        uint64_t iValue;
    };
private:
    static int running_connections_;
    int player_id_;
    ip::tcp::socket sock_;
    unsigned char read_buffer_[MAX_MSG]{};
    unsigned char write_buffer_[MAX_MSG]{};
    //std::string username_;
    deadline_timer timer_;
};

void updateGS();

void handleNewConnection(ConnectionToClient::ptr client, const boost::system::error_code &err);

void runGameStateCycle();

void runServer();


#endif
