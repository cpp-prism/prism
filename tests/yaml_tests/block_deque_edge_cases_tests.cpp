#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismYaml - block format my_deque_int edge cases round trip", "[yaml][block][deque][int]")
{
    SECTION("my_deque_int with negative values block round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {-300, -100, 0, 100, 300};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_deque_int.size() == 5);
        REQUIRE(result->my_deque_int[0] == -300);
        REQUIRE(result->my_deque_int[1] == -100);
        REQUIRE(result->my_deque_int[2] == 0);
        REQUIRE(result->my_deque_int[3] == 100);
        REQUIRE(result->my_deque_int[4] == 300);
    }

    SECTION("my_deque_int single element block round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {42};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_deque_int.size() == 1);
        REQUIRE(result->my_deque_int[0] == 42);
    }

    SECTION("my_deque_int with large boundary values block round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {INT_MAX, INT_MIN};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_deque_int.size() == 2);
        REQUIRE(result->my_deque_int[0] == INT_MAX);
        REQUIRE(result->my_deque_int[1] == INT_MIN);
    }
}
