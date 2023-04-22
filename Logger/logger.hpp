#pragma once

//#include <mutex>
//#include <fstream>
//#include <ios>

#include <string>
#include <string_view>
#include <vector>


#define FMT_HEADER_ONLY
#include <fmt/core.h>

#include "helper.hpp"
#include "handles.hpp"





class Logger {
private:
    std::vector<HandleOutput*> handles;
public:
    level_enum default_level = debug;
    Logger() {}
    Logger(std::initializer_list<HandleOutput*> init_list) : handles(init_list) {}
    void push_handle(HandleOutput* handle) { handles.push_back(handle); }
    HandleOutput* get_handle(size_t idx)   { return handles[idx]; }

    bool log(std::string_view text) {
        auto time =  Helper::get_datatime();
        std::string out_text_s = fmt::format("{0} [{1}] {2}", Helper::level_to_str(default_level),time,text);
        std::string_view out_text = out_text_s;
        for(auto& el : handles) { 
            if (!el->output(out_text))
                return false;
        }
        return true;
    }

};

