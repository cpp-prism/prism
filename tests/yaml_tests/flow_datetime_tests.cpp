#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <chrono>

TEST_CASE("prismYaml - flow format my_datetime and my_timestamp round trips", "[yaml][flow][datetime][timestamp]")
{
    SECTION("my_timestamp non-zero flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_timestamp = initTimestamp(2024, 12, 31, 23, 59, 59);

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_timestamp == obj.my_timestamp);
    }

    SECTION("my_datetime non-epoch flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_datetime = initDatetime(2023, 6, 15, 8, 30, 0);

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        std::time_t orig_ts = std::chrono::system_clock::to_time_t(obj.my_datetime);
        std::time_t res_ts = std::chrono::system_clock::to_time_t(result->my_datetime);
        REQUIRE(res_ts == orig_ts);
    }

    SECTION("my_timestamp = 0 (epoch) flow round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_timestamp = 0;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_timestamp == 0);
    }
}
