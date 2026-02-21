#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map2 null and non-null entries round trip", "[yaml][block][map2][sptr][null]")
{
    SECTION("my_map2 single non-null entry block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 55;
        sub->my_bool = true;
        obj.my_map2["alive"] = sub;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map2.at("alive") != nullptr);
        REQUIRE(result->my_map2.at("alive")->my_int == 55);
        REQUIRE(result->my_map2.at("alive")->my_bool == true);
    }

    SECTION("my_map2 two non-null entries with different fields block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto s1 = std::make_shared<tst_sub_struct>();
        s1->my_int = 10;
        s1->my_string = "s1_yaml";
        obj.my_map2["first"] = s1;

        auto s2 = std::make_shared<tst_sub_struct>();
        s2->my_int = 20;
        s2->my_longlong = 88888LL;
        obj.my_map2["second"] = s2;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map2.at("first")->my_string == "s1_yaml");
        REQUIRE(result->my_map2.at("second")->my_longlong == 88888LL);
    }
}
