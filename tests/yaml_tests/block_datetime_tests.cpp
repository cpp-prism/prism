#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format datetime field round trip", "[yaml][block][datetime]")
{
    SECTION("tst_struct my_datetime block round trip")
    {
        tst_struct obj;
        obj.my_int = 5;
        obj.my_datetime = initDatetime(2024, 6, 15, 12, 30, 0);
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        std::time_t orig_ts = std::chrono::system_clock::to_time_t(obj.my_datetime);
        std::time_t res_ts = std::chrono::system_clock::to_time_t(result->my_datetime);
        REQUIRE(result->my_int == 5);
        REQUIRE(orig_ts == res_ts);
    }

    SECTION("tst_struct my_timestamp (time_t) block round trip")
    {
        tst_struct obj;
        obj.my_int = 6;
        obj.my_timestamp = initTimestamp(2023, 1, 1, 0, 0, 0);
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 6);
        REQUIRE(result->my_timestamp == obj.my_timestamp);
    }

    SECTION("tst_struct both datetime and timestamp block round trip")
    {
        tst_struct obj;
        obj.my_datetime = initDatetime(2025, 12, 31, 23, 59, 59);
        obj.my_timestamp = initTimestamp(2025, 12, 31, 23, 59, 59);
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        std::time_t orig_ts = std::chrono::system_clock::to_time_t(obj.my_datetime);
        std::time_t res_ts = std::chrono::system_clock::to_time_t(result->my_datetime);
        REQUIRE(orig_ts == res_ts);
        REQUIRE(result->my_timestamp == obj.my_timestamp);
    }
}
