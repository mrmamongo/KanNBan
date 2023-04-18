#include <mutex>

#include <vector>
#include <string>
#include <string_view>

#include <fstream>
#include <ios>

#include <type_traits>
#include <ctime>
#include <functional>
#include <iostream>
#include <atomic>
#include <condition_variable>

using int_l = std::conditional_t<sizeof(std::time_t) == sizeof(unsigned),
                                 unsigned, unsigned long long>;

std::mutex mutex;

template<class ... Args>
auto console_out = [](Args&& ... args) { 
    std::lock_guard<std::mutex> lk(mutex);
    (std::cout << ... << args) << std::endl;
};

template<class ... Args>
auto file_out = [](std::string_view filename,Args&& ... args) {
    std::ofstream file(std::string(filename),std::ios::app);
    if (file.is_open())
        (file << ... << args) << std::endl;
    file.close();
};

class Logger {
private:
    template<class ... Args>
    void console_out_(Args&& ... args) {
        console_out<Args...>(std::forward<Args>(args)...);
    }
    template<class ... Args>
    void file_out_(std::string_view filename, Args&& ... args) {
        file_out<Args...>(filename,std::forward<Args>(args)...);
    }
public:
    void logc(std::string text) {
        auto t = static_cast<int_l>(std::time(nullptr));
        console_out_("[LOG ", t, " ] ", text);
    }
    void logf(std::string_view filename, std::string text) {
        auto t = static_cast<int_l>(std::time(nullptr));
        file_out_(filename,"[LOG ", t, " ] ", text);
    }
};


