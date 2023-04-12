//
// Created by teufel on 12.04.23.
//
#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <cstdint>
#include <future>
#include <random>
#include <iostream>
#include "src/utils/thread_pool.hpp"

using namespace kanban_server::utils;


TEST_CASE("thread_pool - dryrun") {
    thread_pool pool;
    REQUIRE_FALSE(pool.inited());
    REQUIRE_FALSE(pool.is_running());
    pool.init(3);
    REQUIRE(pool.inited());
    REQUIRE(pool.is_running());
    REQUIRE(pool.size() == 3);
}

TEST_CASE("thread_pool - testrun") {
    thread_pool pool;
    pool.init(3);
    auto f = pool.async([]() -> int { return 0; });
    REQUIRE(f.get() == 0);
}

TEST_CASE("thread_pool - run") {
    thread_pool pool;
    pool.init(3);
    std::vector<std::future<int>> int_futs;
    std::vector<std::future<bool>> bool_futs;
    for (int i = 0; i != 30; ++i) {
        int_futs.emplace_back(pool.async([](int i) -> int { return i; }, i));
    }
    for (int i = 0; i != 30; ++i) {
        bool_futs.emplace_back(pool.async([](int i) -> bool { return i % 2 == 0; }, i));
    }
    for (int i = 0; i != 30; ++i) {
        REQUIRE(int_futs[i].get() == i);
        REQUIRE(bool_futs[i].get() == (i % 2 == 0));
    }
}

TEST_CASE("thread_pool - terminate before get") {
    thread_pool pool;
    pool.init(3);
    std::vector<std::future<int>> int_futs;
    std::vector<std::future<bool>> bool_futs;
    for (int i = 0; i != 30; ++i) {
        int_futs.emplace_back(pool.async([](int i) -> int { return i; }, i));
    }
    for (int i = 0; i != 30; ++i) {
        bool_futs.emplace_back(pool.async([i]() -> bool { return i % 2 == 0; }));
    }
    pool.terminate();
    REQUIRE(pool.inited());
    REQUIRE_FALSE(pool.is_running());
    for (int i = 0; i != 30; ++i) {
        REQUIRE(int_futs[i].get() == i);
        REQUIRE(bool_futs[i].get() == (i % 2 == 0));
    }
}

TEST_CASE("thread_pool - terminate before async") {
// Seed with a real random value, if available
    std::random_device r;

// Choose a random mean between 1 and 6
    std::default_random_engine eng(r());
    std::uniform_int_distribution<int> uniform_dist(1, 100);

    thread_pool pool;
    pool.init(3);
    std::vector<std::future<int>> int_futs;
    std::vector<std::future<bool>> bool_futs;
    for (int i = 0; i != 30; ++i) {
        int_futs.emplace_back(pool.async(
                [&eng, &uniform_dist](int i) -> int {
                    std::this_thread::sleep_for(std::chrono::milliseconds(uniform_dist(eng)));
                    return i;
                },
                i));
    }
    pool.terminate();
    REQUIRE(pool.inited());
    REQUIRE_FALSE(pool.is_running());
    for (int i = 0; i != 30; ++i) {
        REQUIRE_THROWS(bool_futs.emplace_back(pool.async([i]() -> bool { return i % 2 == 0; })));
    }
    for (int i = 0; i != 30; ++i) {
        REQUIRE(int_futs[i].get() == i);
    }
}

TEST_CASE("thread_pool - cancel before get") {
    thread_pool pool;
    pool.init(3);
    std::vector<std::future<int>> int_futs;
    std::vector<std::future<bool>> bool_futs;
    for (int i = 0; i != 30; ++i) {
        int_futs.emplace_back(pool.async([](int i) -> int { return i; }, i));
    }
    for (int i = 0; i != 30; ++i) {
        bool_futs.emplace_back(pool.async([i]() -> bool { return i % 2 == 0; }));
    }
    pool.cancel();
    REQUIRE(pool.inited());
    REQUIRE_FALSE(pool.is_running());
    for (int i = 0; i != 30; ++i) {
        if (int_futs[i].valid() &&
            int_futs[i].wait_for(std::chrono::milliseconds(50)) == std::future_status::ready) {
            try {
                int ret = int_futs[i].get();
                REQUIRE(i == ret);
            } catch (const std::future_error &e) {
            }
        }
        if (bool_futs[i].valid() &&
            bool_futs[i].wait_for(std::chrono::milliseconds(50)) == std::future_status::ready) {
            try {
                bool ret = bool_futs[i].get();
                REQUIRE((i % 2 == 0) == ret);
            } catch (const std::future_error &e) {
            }
        }
    }
}

TEST_CASE("thread_pool - cancel before async") {
// Seed with a real random value, if available
    std::random_device r;

// Choose a random mean between 1 and 6
    std::default_random_engine eng(r());
    std::uniform_int_distribution<int> uniform_dist(1, 100);

    thread_pool pool;
    pool.init(3);
    std::vector<std::future<int>> int_futs;
    std::vector<std::future<bool>> bool_futs;
    for (int i = 0; i != 30; ++i) {
        int_futs.emplace_back(pool.async(
                [&eng, &uniform_dist](int i) -> int {
                    std::this_thread::sleep_for(std::chrono::milliseconds(uniform_dist(eng)));
                    return i;
                },
                i));
    }
    pool.cancel();
    REQUIRE(pool.inited());
    REQUIRE_FALSE(pool.is_running());
    for (int i = 0; i != 30; ++i) {
        REQUIRE_THROWS(bool_futs.emplace_back(pool.async([i]() -> bool { return i % 2 == 0; })));
    }
    int cancelled_int = 0;
    for (int i = 0; i != 30; ++i) {
        if (int_futs[i].valid() &&
            int_futs[i].wait_for(std::chrono::milliseconds(50)) == std::future_status::ready) {
            try {
                int ret = int_futs[i].get();
                REQUIRE(i == ret);
            } catch (const std::future_error &e) {
                ++cancelled_int;
            }
        }
    }
}