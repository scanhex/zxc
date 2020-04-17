#include "Server.h"
#include "../Utils/BufferIO.h"
#include "../Game/GameState.h"

//global Game State

GameState gameState;
bool running = false;
boost::posix_time::ptime last_tick, now;

int32_t ConnectionToClient::running_connections_ = 0;

io_service service;
ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));
std::mutex g_lock;


ConnectionToClient::ConnectionToClient() : sock_(service),
                                           timer_(service) {
}

void ConnectionToClient::startConnection() {
    sock_.set_option(ip::tcp::no_delay(true));
    player_id_ = running_connections_;
    ++ConnectionToClient::running_connections_;
    //std::cout<<running_connections_<<std::endl;
    timer_.expires_from_now(boost::posix_time::milliseconds(TICK_TIME_SEND_GS));
    timer_.async_wait(BIND_FN1(waitForAllConnections, std::placeholders::_1));
}

ConnectionToClient::ptr ConnectionToClient::newClient() {
    ptr new_client(new ConnectionToClient);
    return new_client;
}

int ConnectionToClient::connectionsNumber() const {
    return ConnectionToClient::running_connections_;
}

void ConnectionToClient::stopConnection() {
    running = false;
    running_connections_ = std::min(0, running_connections_ - 1);
    if (!running_connections_) {
        boost::system::error_code ec;
        sock_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        sock_.close();
    }
}

ip::tcp::socket &ConnectionToClient::sock() {
    return sock_;
}

void ConnectionToClient::handleReadFromSocket(const boost::system::error_code &err, size_t bytes) {
    if (err) {
        stopConnection();
        return;
    }
    if (!running_connections_) return;
    g_lock.lock();
    updateGSbyPlayer();
    g_lock.unlock();
    if (running)
        readFromSocket();
}

void ConnectionToClient::readFromSocket() {
    if (!running_connections_) return;
    async_read(sock_, buffer(read_buffer_),
               BIND_FN2(checkReadComplete, std::placeholders::_1, std::placeholders::_2),
               BIND_FN2(handleReadFromSocket, std::placeholders::_1, std::placeholders::_2));
}

size_t ConnectionToClient::checkReadComplete(const boost::system::error_code &err, size_t bytes) {
    if (err) {
        stopConnection();
        return 0;
    }
    bool done = (bytes >= MSG_FROM_CLIENT_SIZE);
    return done ? 0 : 1;
}

void ConnectionToClient::handleWriteToSocket(const boost::system::error_code &err, size_t bytes) {
    if (err) {
        stopConnection();
        return;
    }
    if (!running_connections_) return;
    if (gameState.gameIsFinished()) stopConnection(); //if game is finished

    timer_.expires_from_now(boost::posix_time::millisec(TICK_TIME_SEND_GS));
    timer_.async_wait(BIND_FN(writeToSocket));
}

void ConnectionToClient::writeToSocket() {
    g_lock.lock();
    writeGStoBuffer();
    g_lock.unlock();
    sock_.async_write_some(buffer(write_buffer_, MSG_FROM_SERVER_SIZE),
                           BIND_FN2(handleWriteToSocket, std::placeholders::_1, std::placeholders::_2));
}

void ConnectionToClient::waitForAllConnections(const boost::system::error_code &err) {
    if (err) {
        std::cout << err.message() << std::endl;
        throw;
    }
    if (running_connections_ == PLAYERS_REQUIRED) {
        writeToSocket();
        readFromSocket();
    } else {
        timer_.expires_from_now(boost::posix_time::milliseconds(TICK_TIME_SEND_GS));
        timer_.async_wait(BIND_FN1(waitForAllConnections, std::placeholders::_1));
    }
}

void ConnectionToClient::updateGSbyPlayer() {
    uint8_t actionId = BufferIO::readUInt8(0, read_buffer_);
    EventName eventName = Event::intToEventName(actionId);
    Player player = player_id_ == 0 ? Player::First : Player::Second;

    Event event(eventName, player);

    if(event.eventName_ == EventName::move){
        event.x_ = BufferIO::readDouble(1, read_buffer_);
        event.y_ = BufferIO::readDouble(9, read_buffer_);
    }

    gameState.applyEvent(event);

    std::cout << "ME: " << gameState.getHealthPoints(Player::First) << '\n';
    std::cout << "SASHKA: " << gameState.getHealthPoints(Player::Second) << '\n';
    std::cout << '\n';
}

void ConnectionToClient::writeGStoBuffer() {
    Player first = Player::First, second = Player::Second;
    if(player_id_ == 1) std::swap(first, second);
    Point firstPos = gameState.getPosition(first);
    Point secondPos = gameState.getPosition(second);

    // my: {hp, x, y}
    BufferIO::writeDouble(gameState.getHealthPoints(first), 0, write_buffer_);
    BufferIO::writeDouble(gameState.getHealthPoints(second), 8, write_buffer_);
    BufferIO::writeDouble(firstPos.x_, 8, write_buffer_);
    BufferIO::writeDouble(firstPos.y_, 16, write_buffer_);

    // his: {hp, x, y}
    BufferIO::writeDouble(gameState.getHealthPoints(second), 24, write_buffer_);
    BufferIO::writeDouble(secondPos.x_, 32, write_buffer_);
    BufferIO::writeDouble(secondPos.y_, 40, write_buffer_);
}


void handleNewConnection(ConnectionToClient::ptr client, const boost::system::error_code &err) {
    if (err) {
        std::cout << err.message() << std::endl;
        throw;
    }
    client->startConnection();
    ConnectionToClient::ptr new_client = ConnectionToClient::newClient();
    if (client->connectionsNumber() < PLAYERS_REQUIRED)
        acceptor.async_accept(new_client->sock(), std::bind(handleNewConnection, new_client, std::placeholders::_1));
}

void updateGS() {
    // Максим -- хорош
    gameState.update(TICK_TIME_GS_UPDATE);
}

void runGameStateCycle() {
    while (running) {
        now = boost::posix_time::microsec_clock::local_time();
        while ((now - last_tick).total_milliseconds() < TICK_TIME_GS_UPDATE) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            now = boost::posix_time::microsec_clock::local_time();
            boost::posix_time::time_duration diff = now - last_tick; // TODO Ruslan: delete if unused
        }
        //TODO may cause some problems in WINDOWS
        last_tick = now;
        g_lock.lock();
        updateGS();
        g_lock.unlock();
    }
}


void runServer() {
    last_tick = boost::posix_time::microsec_clock::local_time();
    ConnectionToClient::ptr client = ConnectionToClient::newClient();
    std::this_thread::sleep_for(std::chrono::milliseconds(TICK_TIME_GS_UPDATE));
    acceptor.async_accept(client->sock(), std::bind(handleNewConnection, client, std::placeholders::_1));
    running = true;
    std::thread gs_cycle(runGameStateCycle);
    service.run();
    gs_cycle.join();
}
