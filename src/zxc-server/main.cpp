#include <iostream>

#include "Server.h"

int main() {
    std::cout << "Running server on port 8080" << std::endl;
    while (true) {
        runServer();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
