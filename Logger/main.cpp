#include "logger.hpp"
#include <thread>
#include <iostream>


using namespace std::chrono_literals;

void foo1() {
    Logger log;
    for (size_t i = 0; i < 10000; i++) {
        log.logc("Always is ok");
        log.logf("test.txt","Always is ok");
    }
}

void foo2() {
    Logger log;
    for (size_t i = 0; i < 10000; i++) {
        log.logc("Always is bad");
        log.logf("test.txt","Always is bad");
    }
}


int main() {
    std::thread t1(foo1);
    std::thread t2(foo2);
    t1.join(); t2.join();
}



/*GlobalLogger logger;
OutputFile* outfile = new OutputFile;


auto printr = [](std::string str) -> int {  std::cout << str; return 0;};

template<typename ... Args>
auto print = [](Args ... args) {
    (std::cout << ... << args);
};


void foo1() {
    for(size_t i = 0; i < 5; i++) {
        logger.log_console("Always is good\n",print<std::string>);
            std::this_thread::sleep_for(50ms);
    }
}

void foo2() {
    for(size_t i = 0; i < 5; i++) {
        logger.log_file("Always is bad\n","file.log",outfile);
        std::this_thread::sleep_for(15ms);
    }
}


int main() {
    logger.set_file(outfile,"log.txt");
    logger.set_console(printr);
    std::cout << logger.is_console() << ' ' << logger.is_file() << std::endl;
    std::this_thread::sleep_for(5ms);

    std::thread t1(foo1);
    std::thread t2(foo2);

    t1.join();
    t2.join();

}*/