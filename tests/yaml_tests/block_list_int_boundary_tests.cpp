#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <climits>

TEST_CASE("prismYaml - block format my_list_int with boundary and edge values round trip", "[yaml][block][list][int][boundary]")
{
    SECTION("my_list_int with INT_MAX and INT_MIN block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int = {INT_MAX, INT_MIN, 0, -1, 1};
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_int.size() == 5);
        auto it = result->my_list_int.begin();
        REQUIRE(*it == INT_MAX); ++it;
        REQUIRE(*it == INT_MIN); ++it;
        REQUIRE(*it == 0); ++it;
        REQUIRE(*it == -1); ++it;
        REQUIRE(*it == 1);
    }

    SECTION("my_list_int single element block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int = {-999};
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_int.size() == 1);
        REQUIRE(result->my_list_int.front() == -999);
    }

    SECTION("my_list_int large list block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        for (int i = 0; i < 10; ++i)
            obj.my_list_int.push_back(i * 5 - 25);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_int.size() == 10);
        int expected = -25;
        for (auto v : result->my_list_int) {
            REQUIRE(v == expected);
            expected += 5;
        }
    }
}
