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
    gameState_.refreshAllUnits();
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
    if (err || exit_flag) {
        stopConnection();
        return;
    }
    assert(bytes == MSG_FROM_CLIENT_SIZE);
    waitForAction();
}

void Client::ConnectionToServer::writeToSocket() {
    if (exit_flag) {
        stopConnection();
        return;
    }
    if (!isConnected()) return;
    writer_.flushBuffer();
    writeActionToBuffer();
    sock_.async_write_some(buffer(writer_.write_buffer_, MSG_FROM_CLIENT_SIZE),
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
    assert(bytes == MSG_WAIT_FROM_SERVER_SIZE);
    uint8_t status = reader_.readUInt8();
    clearEvents();
    if (status) {
        if(status == 2)
            gameState_.reverseIndices();
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
    assert(bytes == MSG_FROM_SERVER_SIZE);
    reader_.flushBuffer();
    char gameIsRunning = reader_.readUInt8();
    if (!gameIsRunning) {
        //TODO handle game result
        if (gameState_.getHealthPoints(Player::First) == 0)
            std::cout << "I lost :(" << std::endl; //
        else
            std::cout << "I won :)" << std::endl;

        stopConnection();
        return;
    }
    parseEventsFromBuffer();
    timer_.expires_from_now(boost::posix_time::millisec(1)); //TODO really needed?
    timer_.wait();
    parseGSFromBuffer();
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

void Client::ConnectionToServer::parseGSFromBuffer() {
    gameState_.deserialize(reader_, Player::First);
}

void Client::ConnectionToServer::parseEventsFromBuffer() {
    size_t sz = reader_.readInt32();
    Hero *hero = gameState_.getHero(Player::Second);
    for (size_t i = 0; i < sz; ++i) {
        uint8_t actionId = reader_.readUInt8();
        auto eventName = static_cast<SerializedEventName>(actionId);
        switch (eventName) {
            case SerializedEventName::Attack: {
                uint8_t attackerID = reader_.readUInt8();
                uint8_t targetID = reader_.readUInt8();
                othersEvents_.push(new FromServerAttackEvent(attackerID, targetID));
                break;
            }
            case SerializedEventName::FirstSkillUse: {
                othersEvents_.push(new FromServerFirstSkillUseEvent(*hero));
                break;
            }
            case SerializedEventName::SecondSkillUse: {
                othersEvents_.push(new FromServerSecondSkillUseEvent(*hero));
                break;
            }
            case SerializedEventName::ThirdSkillUse: {
                othersEvents_.push(new FromServerThirdSkillUseEvent(*hero));
                break;
            }
            case SerializedEventName::Move: {
                double x = reader_.readDouble();
                double y = reader_.readDouble();
                othersEvents_.push(new FromServerMoveEvent(*hero, x, y));
                break;
            }
            case SerializedEventName::Stop: {
                othersEvents_.push(new FromServerStopEvent(*hero));
                break;
            }
            default: {
                assert(false);
                break;
            }
        }
    }
}

void Client::ConnectionToServer::writeActionToBuffer() {
    SerializedEvent *e;
    if (events_.pop(e)) {
        e->serialize(writer_);
        delete e;
    }
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

void Client::ConnectionToServer::clearEvents() {
    Event *e;
    while (events_.pop(e)) {
        delete e;
    }
}

void Client::ConnectionToServer::fireOtherEvents() {
    Event *e;
    if (othersEvents_.empty()) return;
    while (othersEvents_.pop(e)) {
        e->fire();
        delete e;
    }
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

template<typename T>
bool Client::isNotFromServerEvent(T &t) {
    return dynamic_cast<const FromServerEvent *>(&t) == nullptr;
}

void Client::handle(const MoveEvent &event) {
    if (isNotFromServerEvent(event)) {
        connection_->events_.push(new MoveEvent(event));
    }
}

void Client::handle(const StopEvent &event) {
    if (isNotFromServerEvent(event)) {
        connection_->events_.push(new StopEvent(event));
    }
}

void Client::handle(const AttackEvent &event) {
    if (isNotFromServerEvent(event)) {
        connection_->events_.push(new AttackEvent(event));
    }
}

void Client::handle(const FirstSkillUseEvent &event) {
    if (isNotFromServerEvent(event)) {
        connection_->events_.push(new FirstSkillUseEvent(event));
    }
}

void Client::handle(const SecondSkillUseEvent &event) {
    if (isNotFromServerEvent(event)) {
        connection_->events_.push(new SecondSkillUseEvent(event));
    }
}

void Client::handle(const ThirdSkillUseEvent &event) {
    if (isNotFromServerEvent(event)) {
        connection_->events_.push(new ThirdSkillUseEvent(event));
    }
}

void Client::handle(const DrawEvent &event) {
    connection_->fireOtherEvents();
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
