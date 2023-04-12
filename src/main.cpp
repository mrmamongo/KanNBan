//
// Created by teufel on 12.04.23.
//

#include <config/config.hpp>
#include <fstream>
#include <iostream>
#include <fmt/core.h>

kanban_server::config read_config() {
    auto config_file = getenv("CONFIG_FILE");
    if (config_file == nullptr) {
        config_file = "config.json";
    }

    std::ifstream config_fs(config_file);
    return json::parse(config_fs).get<kanban_server::config>();
}


int main() {
    kanban_server::config c = read_config();
    fmt::print("Server {0} configured:\n\t{1}:{2}", c.app_name, c.api_host, c.api_port);
}