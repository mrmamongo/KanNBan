#include "logger.hpp"
#include <thread>
#include <iostream>
 using namespace std::chrono_literals;

GlobalLogger logger;
OutputFile* outfile = new OutputFile;


auto print = [](std::string str) -> int {  std::cout << str; return 0;};



void foo1() {
    for(size_t i = 0; i < 100; i++) {
        logger.log("Always is good\n");
            std::this_thread::sleep_for(50ms);
    }
}

void foo2() {
    for(size_t i = 0; i < 100; i++) {
        logger.log("Always is bad\n");
        std::this_thread::sleep_for(15ms);
    }
}


int main() {
    logger.set_file(outfile,"log.txt");
    logger.set_console(print);
    std::cout << logger.is_console() << ' ' << logger.is_file() << std::endl;
    std::this_thread::sleep_for(5ms);

    std::thread t1(foo1);
    std::thread t2(foo2);

    t1.join();
    t2.join();

}
