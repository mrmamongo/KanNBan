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



using int_l = std::conditional_t<sizeof(std::time_t) == sizeof(unsigned),
                                 unsigned, unsigned long long>;


class OutputFile {
private:
    std::ofstream out;
    std::string filename;
public:
    virtual int set_filename(const std::string_view& filename) { this->filename = std::string(filename.cbegin(),filename.cend()); return 0;}
    virtual std::string get_filename() { return filename; }
    virtual int add_string(const std::string& str) {
        out.open(filename, std::ios::app);
        if (!out.is_open())
            return -1;
        out << str;
        return 0;
    }
    virtual int add_vector(const std::vector<std::string>& v) {
        out.open(filename, std::ios::app);
        if (!out.is_open())
            return -1;
        for (auto& el : v) {
            out << el;
        }
        out.close();
        return 0;
    }
    virtual int clear() {
        out.open(filename, std::ios::trunc);
        if (!out.is_open())
            return -1;
        return 0;
    }
};


class GlobalLogger {
private:
    std::mutex mutex;
    OutputFile* out_file;
    std::function<int(std::string)> out_console;

public:
    void set_file(OutputFile* out, std::string_view filename) {
        out_file = out;
        out_file->set_filename(filename);
    }
    void set_console(std::function<int(std::string)> function) {
        out_console = function;
    }

    bool is_file() { return out_file != nullptr; }
    bool is_console() { return static_cast<bool>(out_console); }

    void log(std::string text) {
        std::lock_guard<std::mutex> lk(mutex);
        if (is_file()) {
            std::vector<std::string> tmp; tmp.reserve(4);
            tmp.push_back("[");
            tmp.push_back(std::to_string(static_cast<int_l>(std::time(nullptr))));
            tmp.push_back("] ");
            tmp.push_back(text);
            out_file->add_vector(tmp);
        }
        if (is_console()) {
            out_console("[");
            out_console(std::to_string(static_cast<int_l>(std::time(nullptr))));
            out_console("] ");
            out_console(text);
        }
    }  




};