#include "logger.hpp"
#include <thread>
#include <iostream>


void foo1() {
    Logger log;
    for (size_t i = 0; i < 1000000000; i++) {
        log.logc("Always is ok");
        log.logf("test.txt","Always is ok");
    }
}

void foo2() {
    Logger log;
    for (size_t i = 0; i < 1000000000; i++) {
        log.logc("Always is bad");
        log.logf("test.txt","Always is bad");
    }
}


int main() {
    std::thread t1(foo1);
    std::thread t2(foo2);
    t1.join(); t2.join();
}
