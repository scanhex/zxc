#include "Server.h"
#include "../Utils/BufferIO.h"
#include "../Game/GameState.h"

GameState gameState; //TODO create class Server
bool running = false;
bool stopped = false;
boost::posix_time::ptime last_tick, now;

int32_t ConnectionToClient::running_connections_ = 0;

io_service service;
ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));
std::mutex g_lock;


ConnectionToClient::ConnectionToClient() : is_connected_(false),
                                           sock_(service),
                                           timer_(service),
                                           stop_timer_(service) {
}

void ConnectionToClient::startConnection() {
    is_connected_ = true;
    conn_checker_.join();
    if (stopped)
        return;
    sock_.set_option(ip::tcp::no_delay(true));
    player_id_ = running_connections_;
    ++ConnectionToClient::running_connections_;
    timer_.expires_from_now(boost::posix_time::milliseconds(TICK_TIME_SEND_GS));
    timer_.async_wait(BIND_FN2(waitForAllConnections, std::placeholders::_1, 0));
}

std::shared_ptr<ConnectionToClient> ConnectionToClient::newClient() {
    std::shared_ptr<ConnectionToClient> new_client(new ConnectionToClient);
    return new_client;
}

int ConnectionToClient::connectionsNumber() const {
    return ConnectionToClient::running_connections_;
}

void ConnectionToClient::stopConnection() {
    stopped = true;
    is_connected_ = false;
    running = false;
    running_connections_ = std::max(0, running_connections_ - 1);
    boost::system::error_code ec;
    stop_timer_.expires_from_now(boost::posix_time::milliseconds(50));
    stop_timer_.wait();
    if (sock_.is_open()) {
        sock_.cancel();
    }
    if (acceptor.is_open()) {
        acceptor.cancel();
        acceptor.close();
    }
    sock_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    sock_.close();
}

ip::tcp::socket &ConnectionToClient::sock() {
    return sock_;
}

void ConnectionToClient::handleReadFromSocket(const boost::system::error_code &err, size_t bytes) {
    if (err || stopped) {
        std::cout << err.message() << std::endl;
        stopConnection();
        return;
    }
    if (!running) return;
    if (!running_connections_) return;
    g_lock.lock();
    updateGSbyPlayer();
    g_lock.unlock();
    readFromSocket();
}

void ConnectionToClient::readFromSocket() {
    if (!running_connections_) return;
    async_read(sock_, buffer(read_buffer_),
               BIND_FN2(checkReadComplete, std::placeholders::_1, std::placeholders::_2),
               BIND_FN2(handleReadFromSocket, std::placeholders::_1, std::placeholders::_2));
}

size_t ConnectionToClient::checkReadComplete(const boost::system::error_code &err, size_t bytes) {
    if (err || stopped) {
        std::cout << err.message() << std::endl;
        stopConnection();
        return 0;
    }
    if (!running) return 0;
    bool done = (bytes >= MSG_FROM_CLIENT_SIZE);
    return done ? 0 : 1;
}

void ConnectionToClient::handleWriteToSocket(const boost::system::error_code &err, size_t bytes) {
    if (err || stopped) {
        std::cout << err.message() << std::endl;
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

void ConnectionToClient::waitForAllConnections(const boost::system::error_code &err, size_t bytes) {
    if (err || stopped) {
        std::cout << err.message() << std::endl;
        stopConnection();
        return;
    }
    if (running_connections_ == PLAYERS_REQUIRED) {
        running = true;
        BufferIO::writeUInt8(1, 0, write_buffer_);
        sock_.write_some(buffer(write_buffer_, MSG_WAIT_FROM_SERVER_SIZE));
        timer_.expires_from_now(boost::posix_time::milliseconds(TICK_TIME_SEND_GS));
        timer_.wait();
        writeToSocket();
        readFromSocket();
    } else {
        timer_.expires_from_now(boost::posix_time::milliseconds(TICK_TIME_SEND_GS));
        timer_.wait();
        BufferIO::writeUInt8(0, 0, write_buffer_);
        sock_.async_write_some(buffer(write_buffer_, MSG_WAIT_FROM_SERVER_SIZE),
                               BIND_FN2(waitForAllConnections, std::placeholders::_1, std::placeholders::_2));
    }
}

void ConnectionToClient::updateGSbyPlayer() {
    uint8_t actionId = BufferIO::readUInt8(0, read_buffer_);
    EventName eventName = Event::intToEventName(actionId);
    Player player = player_id_ == 0 ? Player::First : Player::Second;

    Event event(eventName, player);

    if (event.eventName_ == EventName::move) {
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
    if (player_id_ == 1) std::swap(first, second);
    Point firstPos = gameState.getPosition(first);
    Point secondPos = gameState.getPosition(second);
    Point firstDest = gameState.getDestination(first);
    Point secondDest = gameState.getDestination(second);

    // my: {hp, x, y}
    BufferIO::writeDouble(gameState.getHealthPoints(first), 0, write_buffer_);
    BufferIO::writeDouble(firstPos.x_, 8, write_buffer_);
    BufferIO::writeDouble(firstPos.y_, 16, write_buffer_);
    BufferIO::writeDouble(firstDest.x_, 24, write_buffer_);
    BufferIO::writeDouble(firstDest.y_, 32, write_buffer_);

    // his: {hp, x, y}
    BufferIO::writeDouble(gameState.getHealthPoints(second), 40, write_buffer_);
    BufferIO::writeDouble(secondPos.x_, 48, write_buffer_);
    BufferIO::writeDouble(secondPos.y_, 56, write_buffer_);
    BufferIO::writeDouble(secondDest.x_, 64, write_buffer_);
    BufferIO::writeDouble(secondDest.y_, 72, write_buffer_);
}

void ConnectionToClient::connectionChecker() {
    while (!is_connected_) {
        if (stopped) {
            stopConnection();
            return;
        }
        timer_.expires_from_now(boost::posix_time::milliseconds(1));
        timer_.wait();
    }
}

void ConnectionToClient::startChecker() {
    conn_checker_ = std::thread(&ConnectionToClient::connectionChecker, this);
}

void handleNewConnection(const std::shared_ptr<ConnectionToClient> &client, const boost::system::error_code &err) {
    client->startConnection(); //to stop checker if needed
    if (err || stopped) {
        client->stopConnection();
        std::cout << err.message() << std::endl;
        return;
    }
    if (client->connectionsNumber() < PLAYERS_REQUIRED) {
        std::shared_ptr<ConnectionToClient> new_client = ConnectionToClient::newClient();
        new_client->startChecker();
        acceptor.async_accept(new_client->sock(), std::bind(handleNewConnection, new_client, std::placeholders::_1));
    }
}

void updateGS() {
    // Максим -- худший
    gameState.update(TICK_TIME_GS_UPDATE);
}

void runGameStateCycle() {
    while (!running) {
        if (stopped)
            return;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    while (running && !stopped) {
        now = boost::posix_time::microsec_clock::local_time();
        while ((now - last_tick).total_milliseconds() < TICK_TIME_GS_UPDATE) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            now = boost::posix_time::microsec_clock::local_time();
        }
        //TODO may cause some problems in WINDOWS
        last_tick = now;
        g_lock.lock();
        updateGS();
        g_lock.unlock();
    }
}

void runServer() {
    std::shared_ptr<ConnectionToClient> client = ConnectionToClient::newClient();
    std::this_thread::sleep_for(std::chrono::milliseconds(TICK_TIME_GS_UPDATE));
    client->startChecker();
    acceptor.async_accept(client->sock(), std::bind(handleNewConnection, client, std::placeholders::_1));
    last_tick = boost::posix_time::microsec_clock::local_time();
    std::thread gs_cycle(runGameStateCycle);
    service.run();
    gs_cycle.join();
}
