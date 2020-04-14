#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <thread>
#include <string>
#include <boost/asio.hpp>

static constexpr int MAX_MSG = 1024;
static constexpr int MSG_FROM_SERVER_SIZE = 16;
static constexpr int MSG_FROM_CLIENT_SIZE = 17; //TODO change when add move

using namespace boost::asio;

#define BIND_FN(x)         std::bind(&self_type::x, shared_from_this())
#define BIND_FN1(x, y)      std::bind(&self_type::x, shared_from_this(),y)
#define BIND_FN2(x, y, z)    std::bind(&self_type::x, shared_from_this(),y,z)
#define BIND_FN3(x, y, z, w)  std::bind(&self_type::x, shared_from_this(),y,z,w)

class ConnectionToServer : public std::enable_shared_from_this<ConnectionToServer> {
public:
    typedef std::shared_ptr<ConnectionToServer> ptr;
    typedef ConnectionToServer self_type;

    ConnectionToServer(std::string username);

    static ptr newConnection(const std::string &username);

    void startConnection();

    void stopConnection();

    bool isConnected() const;

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

private:
    /*
     * Functions for working with local Game State and player's actions
     */
    void parseGSFromBuffer();

    void writeActionToBuffer();

private:
    ip::tcp::socket sock_;
    uint8_t read_buffer_[MAX_MSG];
    uint8_t write_buffer_[MAX_MSG];
    bool connected_;
    std::string username_;
    deadline_timer timer_;
};

void runClient();

#endif
