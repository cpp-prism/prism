#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <ctime>

TEST_CASE("prismYaml - block format my_datetime and my_timestamp round trip", "[yaml][block][datetime][timestamp]")
{
    SECTION("my_timestamp non-zero value block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_timestamp = static_cast<std::time_t>(1706745600);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_timestamp == static_cast<std::time_t>(1706745600));
    }

    SECTION("my_datetime epoch block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_datetime = std::chrono::system_clock::time_point{};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_datetime == std::chrono::system_clock::time_point{});
    }

    SECTION("my_datetime and my_timestamp both set block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_timestamp = static_cast<std::time_t>(2000000);
        auto epoch = std::chrono::system_clock::from_time_t(2000000);
        obj.my_datetime = epoch;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_timestamp == static_cast<std::time_t>(2000000));
        REQUIRE(result->my_datetime == epoch);
    }
}
