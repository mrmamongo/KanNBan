#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("Test add", "[unit-test]"){
    REQUIRE(2 + 3 == 5);
    REQUIRE(2. + 3. == 5.);
    REQUIRE(0 + 0 == 0);
}