#include "Server.h"
#include <iostream>

int main() {
    std::cout<<"Server on port 8080"<<std::endl;
    int start_counter = 0;
    while(start_counter < 100) {
        ++start_counter;
        runServer();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
