#include "Server.h"
#include <iostream>

int main() {
    std::cout << "Server on port 8080" << std::endl;
    while (true) {
        runServer();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
