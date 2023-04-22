#pragma once
#include <string>


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


namespace Helper {
    auto get_datatime() { // to do: implement that
        return "";
    }

    std::string level_to_str(level_enum v) {
        switch (v) {
            case off: return "[[off]]"; break;
            case critical: return "[critical]"; break;
            case err: return "[error]"; break;
            case warn: return "[warn]"; break;
            case info: return "[info]"; break;
            case debug: return "[debug]"; break;
            case trace: return "[trace]"; break;   
        }
        return "[[undefinded level]]";
    }
}