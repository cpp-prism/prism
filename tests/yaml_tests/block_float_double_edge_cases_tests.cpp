#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cmath>

TEST_CASE("prismYaml - block format float and double edge case round trips", "[yaml][block][float][double]")
{
    SECTION("my_float negative value block round trip")
    {
        tst_struct obj;
        obj.my_float = -3.14f;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(std::fabs(result->my_float - (-3.14f)) < 1e-4f);
    }

    SECTION("my_double large negative value block round trip")
    {
        tst_struct obj;
        obj.my_double = -9.87654321e10;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(std::fabs(result->my_double - (-9.87654321e10)) / 9.87654321e10 < 1e-6);
    }

    SECTION("my_float and my_double both zero block round trip")
    {
        tst_struct obj;
        obj.my_float = 0.0f;
        obj.my_double = 0.0;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_float == 0.0f);
        REQUIRE(result->my_double == 0.0);
    }

    SECTION("my_double precise fractional value block round trip")
    {
        tst_struct obj;
        obj.my_double = 3.141592653589793;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(std::fabs(result->my_double - 3.141592653589793) < 1e-9);
    }
}
