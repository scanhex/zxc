#include "Client.h"
#include "../Utils/BufferIO.h"
#include <boost/lockfree/queue.hpp>

extern boost::lockfree::queue<Event> events;
extern bool exit_flag;

Client::ConnectionToServer::ConnectionToServer(GameState &gameState) : sock_{service},
                                                                       timer_{service},
                                                                       stop_timer_{service},
                                                                       gameState_{gameState} {}

std::shared_ptr<Client::ConnectionToServer> Client::ConnectionToServer::newConnection(GameState &gameState) {
    std::shared_ptr<ConnectionToServer> new_(new ConnectionToServer(gameState));
    return new_;
}

void Client::ConnectionToServer::startConnection() {
    sock_.async_connect(ep, BIND_FN1(handleConnection, std::placeholders::_1));
}


void Client::ConnectionToServer::stopConnection() {
    if (!connected_) return;
    connected_ = false;
    std::cout << "stopping " << std::endl;
    sock_.close();
}

bool Client::ConnectionToServer::isConnected() const {
    return connected_;
}

void Client::ConnectionToServer::handleConnection(const boost::system::error_code &err) {
    if (err || exit_flag) {
        stopConnection();
        return;
    }
    connected_ = true;
    sock_.set_option(ip::tcp::no_delay(true));
    waitForGameStart();
}

void Client::ConnectionToServer::runGame() {
    std::cout << "Game start!" << std::endl;
    readFromSocket();
    waitForAction();
}

void Client::ConnectionToServer::waitForGameStart() {
    if (exit_flag) {
        stopConnection();
        return;
    }
    async_read(sock_, buffer(read_buffer_),
               BIND_FN2(checkWaitReadComplete, std::placeholders::_1, std::placeholders::_2),
               BIND_FN2(handleWaitRead, std::placeholders::_1, std::placeholders::_2));
}

void Client::ConnectionToServer::handleWriteToSocket(const boost::system::error_code &err, size_t bytes) {
    if (err || exit_flag) {
        stopConnection();
        return;
    }
    waitForAction();
}

void Client::ConnectionToServer::writeToSocket() {
    if (exit_flag) {
        stopConnection();
        return;
    }
    if (!isConnected()) return;
    writeActionToBuffer();
    sock_.async_write_some(buffer(write_buffer_, MSG_FROM_CLIENT_SIZE),
                           BIND_FN2(handleWriteToSocket, std::placeholders::_1, std::placeholders::_2));
}

size_t Client::ConnectionToServer::checkWaitReadComplete(const boost::system::error_code &err, size_t bytes) {
    if (err || exit_flag) {
        stopConnection();
        return 0;
    }
    bool done = (bytes >= MSG_WAIT_FROM_SERVER_SIZE);
    return done ? 0 : 1;
}

void Client::ConnectionToServer::handleWaitRead(const boost::system::error_code &err, size_t bytes) {
    if (err || exit_flag) {
        stopConnection();
        return;
    }
    uint8_t status = BufferIO::readUInt8(0, read_buffer_);
    if (status) {
        runGame();
    } else {
        waitForGameStart();
    }
}

void Client::ConnectionToServer::handleReadFromSocket(const boost::system::error_code &err, size_t bytes) {
    if (err || exit_flag) {
        stopConnection();
        return;
    }
    parseGSFromBuffer();
    if (gameState_.gameIsFinished()) {
        //TODO handle game result
        if (gameState_.getHealthPoints(Player::First) == 0)
            std::cout << "I lost :(" << std::endl; //
        else
            std::cout << "I won :)" << std::endl;

        stopConnection();
    }
    readFromSocket();
}

void Client::ConnectionToServer::readFromSocket() {
    if (!isConnected()) return;
    async_read(sock_, buffer(read_buffer_),
               BIND_FN2(checkReadComplete, std::placeholders::_1, std::placeholders::_2),
               BIND_FN2(handleReadFromSocket, std::placeholders::_1, std::placeholders::_2));
}

size_t Client::ConnectionToServer::checkReadComplete(const boost::system::error_code &err, size_t bytes) {
    if (err || exit_flag) {
        stopConnection();
        return 0;
    }
    bool done = (bytes >= MSG_FROM_SERVER_SIZE);
    return done ? 0 : 1;
}

void Client::ConnectionToServer::updateGS(double hp1, double pos_x1, double pos_y1, double dest_x1, double dest_y1,
                                          double hp2, double pos_x2, double pos_y2, double dest_x2, double dest_y2) {
    gameState_.setHealthPoints(hp1, Player::First);
    gameState_.setPosition(pos_x1, pos_y1, Player::First);
    gameState_.setDestination(dest_x1, dest_y1, Player::First);

    gameState_.setHealthPoints(hp2, Player::Second);
    gameState_.setPosition(pos_x2, pos_y2, Player::Second);
    gameState_.setDestination(dest_x2, dest_y2, Player::Second);
}

void Client::ConnectionToServer::parseGSFromBuffer() {
    double hp1 = BufferIO::readDouble(0, read_buffer_);
    double pos_x1 = BufferIO::readDouble(8, read_buffer_);
    double pos_y1 = BufferIO::readDouble(16, read_buffer_);
    double dest_x1 = BufferIO::readDouble(24, read_buffer_);
    double dest_y1 = BufferIO::readDouble(32, read_buffer_);

    double hp2 = BufferIO::readDouble(40, read_buffer_);
    double pos_x2 = BufferIO::readDouble(48, read_buffer_);
    double pos_y2 = BufferIO::readDouble(56, read_buffer_);
    double dest_x2 = BufferIO::readDouble(64, read_buffer_);
    double dest_y2 = BufferIO::readDouble(72, read_buffer_);

    updateGS(hp1, pos_x1, pos_y1, dest_x1, dest_y1, hp2, pos_x2, pos_y2, dest_x2, dest_y2);
}

void Client::ConnectionToServer::writeActionToBuffer() {
    Event e;
    events.pop(e);

    BufferIO::writeUInt8(e.eventNameToInt(), 0, write_buffer_);

    if (e.eventName_ == EventName::move) {
        BufferIO::writeDouble(e.x_, 1, write_buffer_);
        BufferIO::writeDouble(e.y_, 9, write_buffer_);
    }
}

void Client::ConnectionToServer::waitForAction() {
    if (exit_flag) {
        stopConnection();
        return;
    }
    if (events.empty()) {
        timer_.expires_from_now(boost::posix_time::millisec(1));
        timer_.async_wait(BIND_FN(waitForAction));
    } else {
        writeToSocket();
    }
}

void Client::ConnectionToServer::runService() {
    service.run();
}

void Client::checkServerResponse() {
    if (!connection_->isConnected()) return;
    now = boost::posix_time::microsec_clock::local_time();
    if ((now - last_update).total_milliseconds() > SERVER_RESPONSE_TIME) {
        connection_->stopConnection();
        return;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    now = boost::posix_time::microsec_clock::local_time();
}

void Client::run() {
    connection_->startConnection();
    std::cout << "Connected " << std::endl;
    last_update = boost::posix_time::microsec_clock::local_time();
    std::thread response_checker(&Client::checkServerResponse, this);
    connection_->runService();
    response_checker.join();
    std::cout << "Disonnected " << std::endl;
}

Client::Client(GameState &gameState) : connection_(ConnectionToServer::newConnection(gameState)) {}

void runClient(GameState &gameState) {
    Client client(gameState);
    client.run();
}

//TODO refactor

