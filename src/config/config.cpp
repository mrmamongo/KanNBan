//
// Created by teufel on 12.04.23.
//

#include "config.hpp"

namespace kanban_server {
    void from_json(const json &j, config &c) {
        j.at("app_name").get_to(c.app_name);
        j.at("api_host").get_to(c.api_host);
        j.at("api_port").get_to(c.api_port);
    }
}