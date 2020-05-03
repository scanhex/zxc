#include "Client.h"
#include "Utils/Utils.h"

extern bool exit_flag;

Client::ConnectionToServer::ConnectionToServer(GameState &gameState) : sock_{service_},
                                                                       timer_{service_},
                                                                       stop_timer_{service_},
                                                                       gameState_{gameState} {}

std::shared_ptr<Client::ConnectionToServer> Client::ConnectionToServer::newConnection(GameState &gameState) {
    std::shared_ptr<ConnectionToServer> new_(new ConnectionToServer(gameState));
    return new_;
}

void Client::ConnectionToServer::startConnection() {
    sock_.async_connect(ep_, BIND_FN1(handleConnection, std::placeholders::_1));
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
    reader_.flushBuffer();
    async_read(sock_, buffer(reader_.read_buffer_),
               BIND_FN2(checkWaitReadComplete, std::placeholders::_1, std::placeholders::_2),
               BIND_FN2(handleWaitRead, std::placeholders::_1, std::placeholders::_2));
}

void Client::ConnectionToServer::handleWriteToSocket(const boost::system::error_code &err, size_t bytes) {
    ignore(bytes);
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
    writer_.flushBuffer();
    sock_.async_write_some(buffer(writer_.write_buffer_, MSG_FROM_CLIENT_SIZE),
                           BIND_FN2(handleWriteToSocket, std::placeholders::_1, std::placeholders::_2));
}

size_t Client::ConnectionToServer::checkWaitReadComplete(const boost::system::error_code &err, size_t bytes) {
    ignore(bytes);
    if (err || exit_flag) {
        stopConnection();
        return 0;
    }
    bool done = (bytes >= MSG_WAIT_FROM_SERVER_SIZE);
    return done ? 0 : 1;
}

void Client::ConnectionToServer::handleWaitRead(const boost::system::error_code &err,  size_t bytes) {
    ignore(bytes);
    if (err || exit_flag) {
        stopConnection();
        return;
    }
    uint8_t status = reader_.readUInt8();
    if (status) {
        runGame();
    } else {
        waitForGameStart();
    }
}

void Client::ConnectionToServer::handleReadFromSocket(const boost::system::error_code &err, size_t bytes) {
    ignore(bytes);
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
    reader_.flushBuffer();
    async_read(sock_, buffer(reader_.read_buffer_),
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
    double hp1 = reader_.readDouble();
    double pos_x1 = reader_.readDouble();
    double pos_y1 = reader_.readDouble();
    double dest_x1 = reader_.readDouble();
    double dest_y1 = reader_.readDouble();

    double hp2 = reader_.readDouble();
    double pos_x2 = reader_.readDouble();
    double pos_y2 = reader_.readDouble();
    double dest_x2 = reader_.readDouble();
    double dest_y2 = reader_.readDouble();

    updateGS(hp1, pos_x1, pos_y1, dest_x1, dest_y1, hp2, pos_x2, pos_y2, dest_x2, dest_y2);
}

void Client::ConnectionToServer::writeActionToBuffer() {
    Event *e;
    events_.pop(e);
    e->serialize(writer_);

    delete e;
}

void Client::ConnectionToServer::waitForAction() {
    if (exit_flag) {
        stopConnection();
        return;
    }
    if (events_.empty()) {
        timer_.expires_from_now(boost::posix_time::millisec(1));
        timer_.async_wait(BIND_FN(waitForAction));
    } else {
        writeToSocket();
    }
}

void Client::ConnectionToServer::runService() {
    service_.run();
}

void Client::checkServerResponse() {
    if (!connection_->isConnected()) return;
    now_ = boost::posix_time::microsec_clock::local_time();
    if ((now_ - last_update_).total_milliseconds() > SERVER_RESPONSE_TIME) {
        connection_->stopConnection();
        return;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    now_ = boost::posix_time::microsec_clock::local_time();
}

void Client::handle(const MoveEvent &event) {
    connection_->events_.push(new MoveEvent(event));
}

void Client::handle(const ShortCoilUseEvent &event) {
    connection_->events_.push(new ShortCoilUseEvent(event));
}

void Client::handle(const MidCoilUseEvent &event) {
    connection_->events_.push(new MidCoilUseEvent(event));
}

void Client::handle(const LongCoilUseEvent &event) {
    connection_->events_.push(new LongCoilUseEvent(event));
}

void Client::run() {
    connection_->startConnection();
    std::cout << "Connected " << std::endl;
    last_update_ = boost::posix_time::microsec_clock::local_time();
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
