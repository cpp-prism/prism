#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - block format my_map entries with bool and float/double fields round trip", "[yaml][block][map][bool][float][double]")
{
    SECTION("my_map entry with bool and small double block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct mv;
        mv.my_int = 10;
        mv.my_list_int.clear();
        mv.my_list_std_string.clear();
        mv.my_bool = true;
        mv.my_float = 1.25f;
        mv.my_double = 0.003;
        obj.my_map["block_float"] = std::move(mv);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map["block_float"].my_bool == true);
        REQUIRE(result->my_map["block_float"].my_float == Catch::Approx(1.25f).epsilon(1e-5));
        REQUIRE(result->my_map["block_float"].my_double == Catch::Approx(0.003).epsilon(1e-9));
    }

    SECTION("my_map 2 entries with negative and positive float block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct mv1;
        mv1.my_int = 10;
        mv1.my_list_int.clear();
        mv1.my_list_std_string.clear();
        mv1.my_double = 0.5;
        obj.my_map["pos"] = std::move(mv1);

        tst_struct mv2;
        mv2.my_int = 20;
        mv2.my_list_int.clear();
        mv2.my_list_std_string.clear();
        mv2.my_double = -0.5;
        obj.my_map["neg"] = std::move(mv2);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map["pos"].my_double == Catch::Approx(0.5).epsilon(1e-9));
        REQUIRE(result->my_map["neg"].my_double == Catch::Approx(-0.5).epsilon(1e-9));
    }
}
