#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - block format my_struct (embedded tst_sub_struct) field combo round trip", "[yaml][block][my_struct][embedded][combo]")
{
    SECTION("my_struct with bool and string fields block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 10;
        obj.my_struct.my_bool = true;
        obj.my_struct.my_string = "embedded_block";
        obj.my_struct.my_longlong = 99999LL;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_struct.my_int == 10);
        REQUIRE(result->my_struct.my_bool == true);
        REQUIRE(result->my_struct.my_string == "embedded_block");
        REQUIRE(result->my_struct.my_longlong == 99999LL);
    }

    SECTION("my_struct with float/double fields block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 20;
        obj.my_struct.my_float = 2.5f;
        obj.my_struct.my_double = 6.28318;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_struct.my_int == 20);
        REQUIRE(result->my_struct.my_float == Catch::Approx(2.5f).epsilon(1e-4));
        REQUIRE(result->my_struct.my_double == Catch::Approx(6.28318).epsilon(1e-9));
    }
}
