#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_timestamp (time_t) and my_datetime edge cases", "[json][struct][timestamp][datetime]")
{
    SECTION("tst_struct my_timestamp = 0 (Unix epoch) round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_timestamp = 0;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_timestamp == 0);
    }

    SECTION("tst_struct my_timestamp = large value round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_timestamp = initTimestamp(2038, 1, 18, 3, 14, 7);  // near 32-bit overflow
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_timestamp == obj.my_timestamp);
    }

    SECTION("tst_struct my_datetime epoch (time_point at 0) round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_datetime = std::chrono::system_clock::from_time_t(0);  // Unix epoch
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        std::time_t orig_ts = std::chrono::system_clock::to_time_t(obj.my_datetime);
        std::time_t res_ts = std::chrono::system_clock::to_time_t(result->my_datetime);

        REQUIRE(result->my_int == 3);
        REQUIRE(orig_ts == res_ts);
    }

    SECTION("tst_struct my_datetime and my_timestamp both set round trip")
    {
        tst_struct obj;
        obj.my_int = 4;
        obj.my_datetime = initDatetime(2000, 1, 1, 0, 0, 0);  // Y2K
        obj.my_timestamp = initTimestamp(2000, 1, 1, 0, 0, 0);
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 4);
        REQUIRE(result->my_timestamp == obj.my_timestamp);
        std::time_t orig_ts = std::chrono::system_clock::to_time_t(obj.my_datetime);
        std::time_t res_ts = std::chrono::system_clock::to_time_t(result->my_datetime);
        REQUIRE(orig_ts == res_ts);
    }
}
