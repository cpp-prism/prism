#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_unordermap as tst_struct field round trip", "[yaml][flow][unordered_map][struct]")
{
    SECTION("tst_struct my_unordermap two entries flow round trip")
    {
        tst_struct obj;
        obj.my_int = 5;

        tst_sub_struct s1;
        s1.my_int = 10;
        s1.my_bool = true;
        s1.my_string = "val_alpha";
        obj.my_unordermap["alpha"] = s1;

        tst_sub_struct s2;
        s2.my_int = 20;
        s2.my_bool = false;
        s2.my_string = "val_beta";
        obj.my_unordermap["beta"] = s2;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_unordermap.size() == 2);
        REQUIRE(result->my_unordermap["alpha"].my_int == 10);
        REQUIRE(result->my_unordermap["alpha"].my_bool == true);
        REQUIRE(result->my_unordermap["alpha"].my_string == "val_alpha");
        REQUIRE(result->my_unordermap["beta"].my_int == 20);
        REQUIRE(result->my_unordermap["beta"].my_bool == false);
        REQUIRE(result->my_unordermap["beta"].my_string == "val_beta");
    }

    SECTION("tst_struct my_unordermap empty flow round trip")
    {
        tst_struct obj;
        obj.my_int = 6;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // my_unordermap is empty by default

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 6);
        REQUIRE(result->my_unordermap.empty());
    }
}
