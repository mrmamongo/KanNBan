#pragma once
#include <string>
#include <mutex>
#include <fstream>
#include <ios>

#define FMT_HEADER_ONLY
#include <fmt/core.h>


/////////////////////////////////////// Interface of all Handles

class HandleOutput {
public:
    virtual bool output(std::string_view) = 0;
    virtual ~HandleOutput() {}
};


class HandleFile : public HandleOutput {
private:
    std::mutex m_mutex;
    std::string m_filename;
    std::ofstream file;
public:
    HandleFile(std::string_view filename) {
         m_filename = std::move(std::string(filename.begin(),filename.end()));
         file = std::ofstream(m_filename,std::ios::app);
    }

    bool output(std::string_view text) override {
        {
            std::lock_guard<std::mutex> lk(m_mutex);
            if (!file.is_open())
                return false;
            file << text;
            file.close();
        }
        return true;
    }
    virtual ~HandleFile() {}
};

class HandleConsole : public HandleOutput {
private:
    std::mutex m_mutex;
public:
    bool output(std::string_view text) override {
        {
            std::lock_guard<std::mutex> lk(m_mutex);
            fmt::print(text);
        }
        return true;
    }
    virtual ~HandleConsole() {}
};
