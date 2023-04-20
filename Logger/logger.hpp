#include <mutex>

#include <string>
#include <string_view>

#include <fstream>
#include <ios>

#define FMT_HEADER_ONLY
#include <fmt/core.h>



#if defined(USE_MUTEX)
std::mutex global_mutex;
#endif

enum level_enum // to do: deal something with that
{
    off = -1,
    critical = 0,
    err = 1,
    warn = 2,
    info = 3,
    debug = 4,
    trace = 5
};

template<class ... Args>
auto console_out = [](Args&& ... args) { 
#if defined(USE_MUTEX)
    std::lock_guard<std::mutex> lk(global_mutex); // guess we don't need that in general 
#endif
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
private: // for private fields
    std::string_view m_filename;
    std::mutex local_mutex;
private: // for private funcs
    template<class Date, class T>
    void console_out_(Date&& date, T&& text) {
        console_out<Date,T>(std::forward<Date>(date),std::forward<T>(text));
    }
    template<class Date, class T>
    void file_out_(std::string_view filename, Date&& date, T&& text) {
        file_out<Date,T>(filename,std::forward<Date>(date),std::forward<T>(text));
    }

    auto get_datetime_() { // TO DO: make well enough datatime function
        return "";
    }
public:
    void set_filename(std::string_view filename) {
        std::lock_guard<std::mutex> lk(local_mutex);
        m_filename = filename;
    }

    bool get_filename(std::string_view& filename) {
        std::lock_guard<std::mutex> lk(local_mutex);
        if (m_filename.empty())
            return false;
            filename = m_filename;
    }

    bool is_filename_empty() {
        std::lock_guard<std::mutex> lk(local_mutex);
        return m_filename.empty();
    }


    void logc(std::string_view text) {
        auto dt = get_datetime_();
        console_out_(dt,text);
    }
    void logf(std::string_view filename, std::string_view text) {
        auto dt = get_datetime_();
        file_out_(filename,dt, text);
    }

    void log(std::string_view filename, std::string_view text) {
        auto dt = get_datetime_();
        file_out_(filename,dt,text);
        console_out_(dt,text);
    }

    bool log(std::string_view text) {
        std::string_view filename;
        {
            std::lock_guard<std::mutex> lk(local_mutex);
            if (m_filename.empty()) // i hope that nobody won't use empty filename 
                return false; // throw std::runtime_error("You are logging to file without filename.");
            filename = m_filename; // i hope for call of copy assignment         
        }
        auto dt = get_datetime_();
        file_out_(filename,dt,text);
        console_out_(dt,text);
        return true;
    }



};


