#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - flow format root level bool and float/double combos round trip", "[yaml][flow][bool][float][double][combo]")
{
    SECTION("my_bool true with my_float positive and my_double small flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_bool = true;
        obj.my_float = 1.5f;
        obj.my_double = 0.005;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_float == Catch::Approx(1.5f).epsilon(1e-5));
        REQUIRE(result->my_double == Catch::Approx(0.005).epsilon(1e-9));
    }

    SECTION("my_bool false with my_float and negative double flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_bool = false;
        obj.my_float = 100.0f;
        obj.my_double = -0.0025;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_float == Catch::Approx(100.0f).epsilon(1e-3));
        REQUIRE(result->my_double == Catch::Approx(-0.0025).epsilon(1e-9));
    }
}
