#include "Client.h"
#include "../Game/GameState.h"
#include <utility>

io_service service;
ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);

//local Game State example
GameState gameState;

//temporary counter for cout
int cnt = 0;

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
    if (cnt == 0)
        std::cout << "my hp is " << gameState.getHealthPoints(Player::First) << ", enemy hp is "
                  << gameState.getHealthPoints(Player::Second) << std::endl;
    cnt = (cnt + 1) % 50;
    if (gameState.gameIsFinished()) {

        if (gameState.getHealthPoints(Player::First) == 0)
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
    double hp = readDouble(0);
    gameState.setHealthPoints(hp, Player::First);
    hp = readDouble(8);
    gameState.setHealthPoints(hp, Player::Second);
}

void ConnectionToServer::writeActionToBuffer() {
    double dmg = 100;
  //  std::cout << " damaged " << dmg << std::endl;
    writeDouble(dmg, 0);
}

void ConnectionToServer::writeDouble(double d, int start_idx) {
    binaryDouble u;
    u.dValue = d;
    writeInt64(u.iValue, start_idx);
}

void ConnectionToServer::writeInt32(int32_t d, int start_idx) {
    for (int i = 0; i < 4; i++)
        write_buffer_[start_idx + 3 - i] = (d >> (i * 8));
}

void ConnectionToServer::writeInt64(int64_t d, int start_idx) {
    for (int i = 0; i < 8; i++)
        write_buffer_[start_idx + 7 - i] = (d >> (i * 8));
}


double ConnectionToServer::readDouble(int start_idx) {
    binaryDouble u;
    u.iValue = readInt64(start_idx);
    return u.dValue;
}

int32_t ConnectionToServer::readInt32(int start_idx) {
    int32_t result = 0;
    for (int i = 0; i < 4; i++)
        result = (result << 8) + read_buffer_[start_idx + i];
    return result;
}

int64_t ConnectionToServer::readInt64(int start_idx) {
    int64_t result = 0;
    for (int i = 0; i < 8; i++)
        result = (result << 8) + read_buffer_[start_idx + i];
    return result;
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

