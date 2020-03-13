#include "Server.h"
#include <cmath>

//global Game State example
int max_hp = 1000;
int hp1 = max_hp;
int hp2 = max_hp;
int regen = 1;
bool running = false;

int ConnectionToClient::running_connections_ = 0;

io_service service;
ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));


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
    updateGSbyPlayer();
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
    if (hp1 == 0 || hp2 == 0) stopConnection(); //if game is finished

    timer_.expires_from_now(boost::posix_time::millisec(TICK_TIME_SEND_GS));
    timer_.async_wait(BIND_FN(writeToSocket));
}

void ConnectionToClient::writeToSocket() {
    writeGStoBuffer();
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
    int dmg = read_buffer_[0];
    if (player_id_ == 0 && hp1 > 0) {
        hp2 = std::max(0, hp2 - dmg);
    } else {
        if (hp2 > 0)
            hp1 = std::max(0, hp1 - dmg);
    }
}

void ConnectionToClient::writeGStoBuffer() {
    if (player_id_ == 0) {
        write_buffer_[0] = hp1 / 128;
        write_buffer_[1] = hp1 % 128;
        write_buffer_[2] = hp2 / 128;
        write_buffer_[3] = hp2 % 128;

    } else {
        write_buffer_[0] = hp2 / 128;
        write_buffer_[1] = hp2 % 128;
        write_buffer_[2] = hp1 / 128;
        write_buffer_[3] = hp1 % 128;
    }
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
    if (hp1 > 0 && hp2 > 0)
        hp1 = std::min(max_hp, hp1 + regen);
    if (hp2 > 0 && hp1 > 0)
        hp2 = std::min(max_hp, hp2 + regen);
}

void runGameStateCycle() {
    while (running) {
        updateGS();
        std::this_thread::sleep_for(std::chrono::milliseconds(TICK_TIME_GS_UPDATE));
    }
}


void runServer() {
    ConnectionToClient::ptr client = ConnectionToClient::newClient();
    acceptor.async_accept(client->sock(), std::bind(handleNewConnection, client, std::placeholders::_1));
    running = true;
    std::thread gs_cycle(runGameStateCycle);
    service.run();
    gs_cycle.join();
}
