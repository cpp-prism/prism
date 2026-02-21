#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <ctime>

TEST_CASE("prismJson - my_datetime and my_timestamp round trip", "[json][datetime][timestamp]")
{
    SECTION("my_timestamp non-zero value round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_timestamp = static_cast<std::time_t>(1706745600);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_timestamp == static_cast<std::time_t>(1706745600));
    }

    SECTION("my_datetime epoch round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_datetime = std::chrono::system_clock::time_point{};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_datetime == std::chrono::system_clock::time_point{});
    }

    SECTION("my_datetime and my_timestamp both set round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_timestamp = static_cast<std::time_t>(1000000);
        auto epoch = std::chrono::system_clock::from_time_t(1000000);
        obj.my_datetime = epoch;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_timestamp == static_cast<std::time_t>(1000000));
        REQUIRE(result->my_datetime == epoch);
    }
}
