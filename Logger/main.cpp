#include "logger.hpp"

#include <thread>

#include <chrono> // for ms
using namespace std::literals::chrono_literals;

HandleConsole* h_file = new HandleConsole();

void test1() {
    Logger logger;
    logger.push_handle(h_file);

    for(size_t i = 0; i < 100; i++) {
        logger.log("Always is good\n");
        std::this_thread::sleep_for(1ms);
    }
}

void test2() {
    Logger logger;
    logger.push_handle(h_file);

    for(size_t i = 0; i < 100; i++) {
        logger.log("Always is bad\n");
        std::this_thread::sleep_for(1ms);
    }
}


int main() {
    std::thread t1(test1);
    std::thread t2(test2);
    t1.join();
    t2.join();
}
