#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_list_std_string large and varied content round trip", "[yaml][block][list_std_string][large]")
{
    SECTION("my_list_std_string six elements block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        for (int i = 0; i < 6; ++i) {
            obj.my_list_std_string.push_back("elem" + std::to_string(i));
        }

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_std_string.size() == 6);
        REQUIRE(result->my_list_std_string.front() == "elem0");
        REQUIRE(result->my_list_std_string.back() == "elem5");
    }

    SECTION("my_list_std_string with different length strings block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_list_std_string = {"x", "yy", "zzz"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_std_string.size() == 3);
        REQUIRE(result->my_list_std_string.front() == "x");
        REQUIRE(result->my_list_std_string.back() == "zzz");
    }

    SECTION("my_list_std_string with numeric strings block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_list_std_string = {"10", "20", "30"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_std_string.size() == 3);
        REQUIRE(result->my_list_std_string.front() == "10");
        REQUIRE(result->my_list_std_string.back() == "30");
    }
}
