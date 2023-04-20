#include <mutex>

#include <string>
#include <string_view>

#include <fstream>
#include <ios>

#define FMT_HEADER_ONLY
#include <fmt/core.h>




std::mutex mutex;

template<class ... Args>
auto console_out = [](Args&& ... args) { 
    //std::lock_guard<std::mutex> lk(mutex); // guess we don't need that
    fmt::print("[LOG][Date {0}] \"{1}\"\n",args...);
};

template<class ... Args>
auto file_out = [](std::string_view filename,Args&& ... args) {
    //auto out = fmt::output_file("guide.txt"); out.print("[LOG][Date {0}] \"{1}\"\n",args...); // TO DO: implement that
    std::ofstream file(std::string(filename),std::ios::app);
    if (file.is_open())
        (file << fmt::format("[LOG][Date {0}] \"{1}\"\n",args...));
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
public: // TO DO: add normal date
    void logc(std::string text) {
        console_out_("",text);
    }
    void logf(std::string_view filename, std::string text) {
        file_out_(filename,"", text);
    }
};


