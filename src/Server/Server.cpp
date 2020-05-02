#include "Server.h"

int32_t Server::ConnectionToClient::running_connections_ = 0;

Server::ConnectionToClient::ConnectionToClient(io_service &service, GameState &gs,
                                               bool &running, bool &stopped, ip::tcp::acceptor &ac, std::mutex &lock) :
        gameState(gs),
        running_(running),
        stopped_(stopped),
        acceptor_(ac),
        gs_lock_(lock),
        is_connected_(false),
        sock_(service),
        timer_(service),
        stop_timer_(service) {}

void Server::ConnectionToClient::startConnection() {
    is_connected_ = true;
    conn_checker_.join();
    if (stopped_)
        return;
    sock_.set_option(ip::tcp::no_delay(true));
    player_id_ = running_connections_;
    ++ConnectionToClient::running_connections_;
    timer_.expires_from_now(boost::posix_time::milliseconds(TICK_TIME_SEND_GS));
    timer_.async_wait(BIND_FN2(waitForAllConnections, std::placeholders::_1, 0));
}

std::shared_ptr<Server::ConnectionToClient> Server::newClient() {
    std::shared_ptr<ConnectionToClient> new_client(
            new ConnectionToClient(service_, gameState, running_, stopped_, acceptor_, gs_lock_));
    return new_client;
}

int Server::ConnectionToClient::connectionsNumber() {
    return ConnectionToClient::running_connections_;
}

void Server::ConnectionToClient::stopConnection() {
    stopped_ = true;
    is_connected_ = false;
    running_ = false;
    running_connections_ = std::max(0, running_connections_ - 1);
    boost::system::error_code ec;
    stop_timer_.expires_from_now(boost::posix_time::milliseconds(50));
    stop_timer_.wait();
    if (sock_.is_open()) {
        sock_.cancel();
    }
    if (acceptor_.is_open()) {
        acceptor_.cancel();
        acceptor_.close();
    }
    sock_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    sock_.close();
}

ip::tcp::socket &Server::ConnectionToClient::sock() {
    return sock_;
}

void Server::ConnectionToClient::handleReadFromSocket(const boost::system::error_code &err, size_t bytes) {
    if (err || stopped_) {
        std::cout << err.message() << std::endl;
        stopConnection();
        return;
    }
    if (!running_) return;
    if (!running_connections_) return;
    gs_lock_.lock();
    updateGSbyPlayer();
    gs_lock_.unlock();
    readFromSocket();
}

void Server::ConnectionToClient::readFromSocket() {
    if (!running_connections_) return;
    reader_.flushBuffer();
    async_read(sock_, buffer(reader_.read_buffer_),
               BIND_FN2(checkReadComplete, std::placeholders::_1, std::placeholders::_2),
               BIND_FN2(handleReadFromSocket, std::placeholders::_1, std::placeholders::_2));
}

size_t Server::ConnectionToClient::checkReadComplete(const boost::system::error_code &err, size_t bytes) {
    if (err || stopped_) {
        std::cout << err.message() << std::endl;
        stopConnection();
        return 0;
    }
    if (!running_) return 0;
    bool done = (bytes >= MSG_FROM_CLIENT_SIZE);
    return done ? 0 : 1;
}

void Server::ConnectionToClient::handleWriteToSocket(const boost::system::error_code &err, size_t bytes) {
    if (err || stopped_) {
        std::cout << err.message() << std::endl;
        stopConnection();
        return;
    }
    if (!running_connections_) return;
    if (gameState.gameIsFinished()) stopConnection(); //if game is finished

    timer_.expires_from_now(boost::posix_time::millisec(TICK_TIME_SEND_GS));
    timer_.async_wait(BIND_FN(writeToSocket));
}

void Server::ConnectionToClient::writeToSocket() {
    gs_lock_.lock();
    writeGStoBuffer();
    gs_lock_.unlock();
    writer_.flushBuffer();
    sock_.async_write_some(buffer(writer_.write_buffer_, MSG_FROM_SERVER_SIZE),
                           BIND_FN2(handleWriteToSocket, std::placeholders::_1, std::placeholders::_2));
}

void Server::ConnectionToClient::waitForAllConnections(const boost::system::error_code &err, size_t bytes) {
    if (err || stopped_) {
        std::cout << err.message() << std::endl;
        stopConnection();
        return;
    }
    if (running_connections_ == PLAYERS_REQUIRED) {
        running_ = true;
        writer_.flushBuffer();
        writer_.writeUInt8(1);
        sock_.write_some(buffer(writer_.write_buffer_, MSG_WAIT_FROM_SERVER_SIZE));
        timer_.expires_from_now(boost::posix_time::milliseconds(TICK_TIME_SEND_GS));
        timer_.wait();
        writeToSocket();
        readFromSocket();
    } else {
        timer_.expires_from_now(boost::posix_time::milliseconds(TICK_TIME_SEND_GS));
        timer_.wait();
        writer_.flushBuffer();
        writer_.writeUInt8(0);
        sock_.async_write_some(buffer(writer_.write_buffer_, MSG_WAIT_FROM_SERVER_SIZE),
                               BIND_FN2(waitForAllConnections, std::placeholders::_1, std::placeholders::_2));
    }
}

void Server::ConnectionToClient::updateGSbyPlayer() {
    reader_.flushBuffer();
    uint8_t actionId = reader_.readUInt8();
    EventName eventName = static_cast<EventName>(actionId);
    Player player = player_id_ == 0 ? Player::First : Player::Second;

    Event event(eventName, player);

    if (event.eventName_ == EventName::move) {
        event.x_ = reader_.readDouble();
        event.y_ = reader_.readDouble();
    }

    gameState.applyEvent(event);

    std::cout << "ME: " << gameState.getHealthPoints(Player::First) << '\n';
    std::cout << "SASHKA: " << gameState.getHealthPoints(Player::Second) << '\n';
    std::cout << '\n';
}

void Server::ConnectionToClient::writeGStoBuffer() {
    Player first = Player::First, second = Player::Second;
    if (player_id_ == 1) std::swap(first, second);
    Point firstPos = gameState.getPosition(first);
    Point secondPos = gameState.getPosition(second);
    Point firstDest = gameState.getDestination(first);
    Point secondDest = gameState.getDestination(second);

    writer_.flushBuffer();

    // my: {hp, x, y}
    writer_.writeDouble(gameState.getHealthPoints(first));
    writer_.writeDouble(firstPos.x_);
    writer_.writeDouble(firstPos.y_);
    writer_.writeDouble(firstDest.x_);
    writer_.writeDouble(firstDest.y_);

    // his: {hp, x, y}
    writer_.writeDouble(gameState.getHealthPoints(second));
    writer_.writeDouble(secondPos.x_);
    writer_.writeDouble(secondPos.y_);
    writer_.writeDouble(secondDest.x_);
    writer_.writeDouble(secondDest.y_);
}

void Server::ConnectionToClient::connectionChecker() {
    while (!is_connected_) {
        if (stopped_) {
            stopConnection();
            return;
        }
        timer_.expires_from_now(boost::posix_time::milliseconds(1));
        timer_.wait();
    }
}

void Server::ConnectionToClient::startChecker() {
    conn_checker_ = std::thread(&ConnectionToClient::connectionChecker, this);
}

void
Server::handleNewConnection(const std::shared_ptr<ConnectionToClient> &client, const boost::system::error_code &err) {
    client->startConnection(); //to stop checker if needed
    if (err || stopped_) {
        client->stopConnection();
        std::cout << err.message() << std::endl;
        return;
    }
    if (client->connectionsNumber() < PLAYERS_REQUIRED) {
        std::shared_ptr<ConnectionToClient> new_client = newClient();
        new_client->startChecker();
        acceptor_.async_accept(new_client->sock(),
                               std::bind(&Server::handleNewConnection, this, new_client, std::placeholders::_1));
    }
}

void Server::updateGS() {
    // Максим -- худший
    gameState.update(TICK_TIME_GS_UPDATE);
}

void Server::runGameStateCycle() {
    while (!running_) {
        if (stopped_)
            return;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    while (running_ && !stopped_) {
        now = boost::posix_time::microsec_clock::local_time();
        while ((now - last_tick).total_milliseconds() < TICK_TIME_GS_UPDATE) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            now = boost::posix_time::microsec_clock::local_time();
        }
        //TODO may cause some problems in WINDOWS
        last_tick = now;
        gs_lock_.lock();
        updateGS();
        gs_lock_.unlock();
    }
}

void runServer() {
    Server server;
    server.run();
}

Server::Server() {}

void Server::run() {
    std::shared_ptr<ConnectionToClient> client = newClient();
    std::this_thread::sleep_for(std::chrono::milliseconds(TICK_TIME_GS_UPDATE));
    client->startChecker();
    acceptor_.async_accept(client->sock(),
                           std::bind(&Server::handleNewConnection, this, client, std::placeholders::_1));
    last_tick = boost::posix_time::microsec_clock::local_time();
    std::thread gs_cycle(&Server::runGameStateCycle, this);
    service_.run();
    gs_cycle.join();
}
