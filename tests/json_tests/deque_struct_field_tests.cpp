#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - deque<int> struct field round trip", "[json][deque][struct]")
{
    SECTION("deque<int> with values round trip")
    {
        tst_struct original;
        original.my_deque_int = {10, 20, 30, 40};

        std::string json = prism::json::toJsonString(original);
        REQUIRE(json.find("\"my_deque_int\"") != std::string::npos);

        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->my_deque_int.size() == 4);
        REQUIRE(result->my_deque_int[0] == 10);
        REQUIRE(result->my_deque_int[1] == 20);
        REQUIRE(result->my_deque_int[2] == 30);
        REQUIRE(result->my_deque_int[3] == 40);
    }

    SECTION("empty deque<int> round trip")
    {
        tst_struct original;
        original.my_deque_int = {};

        std::string json = prism::json::toJsonString(original);
        REQUIRE(json.find("\"my_deque_int\":[]") != std::string::npos);

        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->my_deque_int.empty());
    }

    SECTION("standalone deque<int> round trip")
    {
        std::deque<int> original = {1, 2, 3};
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::deque<int>>(json);
        REQUIRE(result->size() == 3);
        REQUIRE((*result)[0] == 1);
        REQUIRE((*result)[1] == 2);
        REQUIRE((*result)[2] == 3);
    }
}
