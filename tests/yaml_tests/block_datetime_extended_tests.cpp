#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <chrono>

TEST_CASE("prismYaml - block format my_datetime and my_timestamp extended round trips", "[yaml][block][datetime][timestamp]")
{
    SECTION("my_timestamp = 0 (Unix epoch) block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_timestamp = 0;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_timestamp == 0);
    }

    SECTION("my_timestamp large value block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_timestamp = initTimestamp(2030, 6, 15, 12, 0, 0);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_timestamp == obj.my_timestamp);
    }

    SECTION("my_datetime epoch block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_datetime = initDatetime(1970, 1, 1, 0, 0, 0);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        std::time_t orig_ts = std::chrono::system_clock::to_time_t(obj.my_datetime);
        std::time_t res_ts = std::chrono::system_clock::to_time_t(result->my_datetime);
        REQUIRE(res_ts == orig_ts);
    }

    SECTION("my_datetime and my_timestamp both set block round trip")
    {
        tst_struct obj;
        obj.my_int = 4;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_timestamp = initTimestamp(2025, 1, 1, 0, 0, 0);
        obj.my_datetime = initDatetime(2025, 1, 1, 0, 0, 0);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 4);
        REQUIRE(result->my_timestamp == obj.my_timestamp);
        std::time_t orig_ts = std::chrono::system_clock::to_time_t(obj.my_datetime);
        std::time_t res_ts = std::chrono::system_clock::to_time_t(result->my_datetime);
        REQUIRE(res_ts == orig_ts);
    }
}
