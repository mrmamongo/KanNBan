//
// Created by teufel on 12.04.23.
//

#ifndef CMAKEDEMO_CONFIG_HPP
#define CMAKEDEMO_CONFIG_HPP

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace kanban_server {
    struct config {
        std::string app_name;
        std::string api_host;
        int api_port;
    };

    void from_json(const json& j, config& c);
}; // namespace kanban_server


#endif //CMAKEDEMO_CONFIG_HPP
