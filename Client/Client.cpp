#include "Client.h"

#include <utility>

io_service service;
ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);

//local Game State example
int hp1;
int hp2;

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
        readFromSocket();
        writeToSocket();
    } else {
        stopConnection();
        return;
    }
}

void ConnectionToServer::handleWriteToSocket(const boost::system::error_code &err, size_t bytes) {
    int millis = rand() % 1000; //waiting for some action from player
    timer_.expires_from_now(boost::posix_time::millisec(millis));

    timer_.async_wait(BIND_FN(writeToSocket));
}

void ConnectionToServer::writeToSocket() {
    if (!isConnected()) return;
    writeActionToBuffer();
    sock_.async_write_some(buffer(write_buffer_, MSG_FROM_CLIENT_SIZE),
                           BIND_FN2(handleWriteToSocket, std::placeholders::_1, std::placeholders::_2));
}

void ConnectionToServer::handleReadFromSocket(const boost::system::error_code &err, size_t bytes) {
    parseGSFromBuffer();

    std::cout << "my hp is " << hp1 << ", enemy hp is " << hp2 << std::endl;

    if (hp1 == 0 || hp2 == 0) {// if game is finished

        if (hp1 == 0)
            std::cout << username_ << ": I lost :(" << std::endl; //just some game result handling
        else
            std::cout << username_ << ": I won :)" << std::endl;

        stopConnection();
    }
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
    hp1 = read_buffer_[0] * 128 + read_buffer_[1];
    hp2 = read_buffer_[2] * 128 + read_buffer_[3];
}

void ConnectionToServer::writeActionToBuffer() {
    char val = rand() % 128;
    //std::cout << username_ << " damaged " << (int) val << std::endl;
    write_buffer_[0] = val;
}

void runClient() {
    std::string name;
    //std::cin>>name;
    srand(time(nullptr)); //to have different random in client examples
    name = "";
    ConnectionToServer::ptr client = ConnectionToServer::newConnection(name);
    client->startConnection();
    std::cout << "Connect " << name << std::endl;
    service.run();
}

