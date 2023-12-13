#include "Client.h"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

extern bool exit_flag;

Client::ConnectionToServer::ConnectionToServer(GameState &gameState)
    : sock_{service_}, timer_{service_}, stop_timer_{service_}, gameState_{gameState} {
    boost::property_tree::ptree pt;

    try {
        boost::property_tree::ini_parser::read_ini(RESOURCE_DIR "/config.ini", pt);
    } catch (const boost::property_tree::ini_parser_error &e) {
        std::cout << std::string("No configuration file found: ") + e.what() << std::endl;
    }

    std::string serverIP = pt.get<std::string>("SERVER_IP", "51.250.68.234");
    ep_ = {ip::address::from_string(serverIP), 8080};
}

std::shared_ptr<Client::ConnectionToServer> Client::ConnectionToServer::newConnection(GameState &gameState) {
    return std::make_shared<ConnectionToServer>(gameState);
}

void Client::ConnectionToServer::startConnection() {
    std::cout << "Connecting to server at " + ep_.address().to_string() + "..." << std::endl;
    sock_.async_connect(ep_, BIND_FN1(handleConnection, std::placeholders::_1));
}

void Client::ConnectionToServer::stopConnection() {
    if (!connected_) {
        return;
    }
    connected_ = false;
    std::cout << "stopping..." << std::endl;
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
    gameState_.startNewGame();
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
    async_read(
        sock_,
        buffer(reader_.read_buffer_),
        BIND_FN2(checkWaitReadComplete, std::placeholders::_1, std::placeholders::_2),
        BIND_FN2(handleWaitRead, std::placeholders::_1, std::placeholders::_2)
    );
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
    if (!isConnected()) {
        return;
    }
    writer_.flushBuffer();
    writeActionToBuffer();
    sock_.async_write_some(
        buffer(writer_.write_buffer_, MSG_FROM_CLIENT_SIZE),
        BIND_FN2(handleWriteToSocket, std::placeholders::_1, std::placeholders::_2)
    );
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
    if (status > 0) {
        current_ = Player(status - 1);
        if (current_ == Player::Second) {
            gameState_.reverseIndices();
        }
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
    auto status = GameStatus(reader_.readUInt8());
    if (status != GameStatus::InProgress) {
        bool won1 = status == GameStatus::FirstPlayerWon && current_ == Player::First;
        bool won2 = status == GameStatus::SecondPlayerWon && current_ == Player::Second;
        if (won1 || won2) {
            std::cout << "I won :)" << std::endl;
        } else {
            std::cout << "I lost :(" << std::endl;
        }

        stopConnection();
        return;
    }
    parseEventsFromBuffer();
    timer_.expires_from_now(boost::posix_time::millisec(1));  // TODO really needed?
    timer_.wait();
    parseGSFromBuffer();
    readFromSocket();
}

void Client::ConnectionToServer::readFromSocket() {
    if (!isConnected()) {
        return;
    }
    reader_.flushBuffer();
    async_read(
        sock_,
        buffer(reader_.read_buffer_),
        BIND_FN2(checkReadComplete, std::placeholders::_1, std::placeholders::_2),
        BIND_FN2(handleReadFromSocket, std::placeholders::_1, std::placeholders::_2)
    );
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
    if (othersEvents_.empty()) {
        return;
    }
    while (othersEvents_.pop(e)) {
        e->fire();
        delete e;
    }
}

template<typename E, typename>
bool Client::isNotFromServerEvent(E &e) {
    return dynamic_cast<const FromServerEvent *>(&e) == nullptr;
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
    if (isNotFromServerEvent(event) && gameState_.findUnitByID(event.attackerID_)->isHero()) {
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
    if (connection_) {
        connection_->fireOtherEvents();
    }
}

void Client::run() {
    while (true) {
        connection_ = ConnectionToServer::newConnection(gameState_);
        connection_->startConnection();
        connection_->runService();
        std::cout << "Disconnected" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

Client::Client(GameState &gameState) : gameState_{gameState} {}
