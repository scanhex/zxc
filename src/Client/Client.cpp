#include "Client.h"
#include "../Game/GameState.h"
#include "../Utils/BufferIO.h"
#include <boost/lockfree/queue.hpp>
#include <cassert>
#include <utility>

io_service service;
ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);
extern boost::lockfree::queue<Event> events;

//local Game State example
//GameState gameState;

//temporary counter for cout
int32_t cnt = 0;

ConnectionToServer::ConnectionToServer(std::string username) : sock_(service),
                                                               username_(std::move(username)),
                                                               timer_(service) {
}

ConnectionToServer::ptr ConnectionToServer::newConnection(const std::string &username) {
    ptr new_(new ConnectionToServer(username));
    return new_;
}

void ConnectionToServer::startConnection() {
    sock_.async_connect(ep, BIND_FN1(handleConnection, std::placeholders::_1));
}


void ConnectionToServer::stopConnection() {
    if (!connected_) return;
    std::cout << "stopping " << username_ << std::endl;
    connected_ = false;
    sock_.close();
}

bool ConnectionToServer::isConnected() const {
    return connected_;
}

void ConnectionToServer::handleConnection(const boost::system::error_code &err) {
    if (!err) {
        connected_ = true;
        sock_.set_option(ip::tcp::no_delay(true));
        read(sock_, buffer(read_buffer_)); //wait for signal fro server to start
        std::cout << "Game start!" << std::endl;
        readFromSocket();
        writeToSocket();
    } else {
        stopConnection();
        return;
    }
}

void ConnectionToServer::handleWriteToSocket(const boost::system::error_code &err, size_t bytes) {
    while (events.empty()) {}

    writeToSocket();
}

void ConnectionToServer::writeToSocket() {
    if (!isConnected()) return;
    writeActionToBuffer();
    sock_.async_write_some(buffer(write_buffer_, MSG_FROM_CLIENT_SIZE),
                           BIND_FN2(handleWriteToSocket, std::placeholders::_1, std::placeholders::_2));
}

void ConnectionToServer::handleReadFromSocket(const boost::system::error_code &err, size_t bytes) {
    parseGSFromBuffer();
    // TODO
//    if (gameState.gameIsFinished()) {
//
//
//        if (gameState.getHealthPoints(Player::First) == 0)
//            std::cout << username_ << ": I lost :(" << std::endl;
//        else
//            std::cout << username_ << ": I won :)" << std::endl;
//
//        stopConnection();
//    }
    readFromSocket();
}

void ConnectionToServer::readFromSocket() {
    if (!isConnected()) return;
    async_read(sock_, buffer(read_buffer_),
               BIND_FN2(checkReadComplete, std::placeholders::_1, std::placeholders::_2),
               BIND_FN2(handleReadFromSocket, std::placeholders::_1, std::placeholders::_2));
}

size_t ConnectionToServer::checkReadComplete(const boost::system::error_code &err, size_t bytes) {
    if (err) {
        stopConnection();
        return 0;
    }
    bool done = (bytes >= MSG_FROM_SERVER_SIZE);
    return done ? 0 : 1;
}

void ConnectionToServer::parseGSFromBuffer() {
    double hp = BufferIO::readDouble(0, read_buffer_);
//    gameState.setHealthPoints(hp, Player::First);
    hp = BufferIO::readDouble(8, read_buffer_);
//    gameState.setHealthPoints(hp, Player::Second);
}

void ConnectionToServer::writeActionToBuffer() {
    Event e;
    events.pop(e);

    BufferIO::writeUInt8(e.eventNameToInt(), 0, write_buffer_);

    if(e.eventName_ == EventName::move){
        assert(e.x_ && e.y_);

        BufferIO::writeDouble(*e.x_, 1, write_buffer_);
        BufferIO::writeDouble(*e.y_, 9, write_buffer_);
    }
}

void runClient() {
    std::string name;
    ConnectionToServer::ptr client = ConnectionToServer::newConnection(name);
    client->startConnection();
    std::cout << "Connect " << name << std::endl;
    service.run();
}

