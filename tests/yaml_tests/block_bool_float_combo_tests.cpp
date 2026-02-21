#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - block format root level bool and float/double combos round trip", "[yaml][block][bool][float][double][combo]")
{
    SECTION("my_bool true with my_float positive and my_double small block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_bool = true;
        obj.my_float = 2.5f;
        obj.my_double = 0.001;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_float == Catch::Approx(2.5f).epsilon(1e-5));
        REQUIRE(result->my_double == Catch::Approx(0.001).epsilon(1e-9));
    }

    SECTION("my_bool false with my_float zero and my_double large block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_bool = false;
        obj.my_float = 0.0f;
        obj.my_double = 9876.54321;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_float == Catch::Approx(0.0f).epsilon(1e-5));
        REQUIRE(result->my_double == Catch::Approx(9876.54321).epsilon(1e-9));
    }

    SECTION("my_bool true with my_longlong and my_double small block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_bool = true;
        obj.my_longlong = 9876543210LL;
        obj.my_double = 0.001;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_longlong == 9876543210LL);
        REQUIRE(result->my_double == Catch::Approx(0.001).epsilon(1e-9));
    }
}
