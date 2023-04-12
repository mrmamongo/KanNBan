//
// Created by teufel on 12.04.23.
//

#include <config/config.hpp>
#include <fstream>
#include <fmt/core.h>
#include <utils/thread_pool.hpp>

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
    fmt::println("Server {0} configured:\n\t{1}:{2}", c.app_name, c.api_host, c.api_port);

    kanban_server::utils::thread_pool pool;

    pool.init(std::thread::hardware_concurrency());

    pool.async([]() {
        fmt::println("Started worker");
    });
}