#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_unordermap with multiple sub_struct entries round trip", "[yaml][block][unordermap][large]")
{
    SECTION("my_unordermap four entries with different int values block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_unordermap.clear();

        for (int i = 1; i <= 4; ++i) {
            tst_sub_struct sub;
            sub.my_int = i * 10;
            obj.my_unordermap["key" + std::to_string(i)] = sub;
        }

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_unordermap.size() == 4);
        REQUIRE(result->my_unordermap.at("key1").my_int == 10);
        REQUIRE(result->my_unordermap.at("key4").my_int == 40);
    }

    SECTION("my_unordermap sub_struct with bool and string block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_unordermap.clear();

        tst_sub_struct sub1;
        sub1.my_int = 1;
        sub1.my_bool = true;
        sub1.my_string = "alpha";
        obj.my_unordermap["a"] = sub1;

        tst_sub_struct sub2;
        sub2.my_int = 2;
        sub2.my_bool = false;
        sub2.my_string = "beta";
        obj.my_unordermap["b"] = sub2;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_unordermap.size() == 2);
        REQUIRE(result->my_unordermap.at("a").my_bool == true);
        REQUIRE(result->my_unordermap.at("a").my_string == "alpha");
        REQUIRE(result->my_unordermap.at("b").my_bool == false);
        REQUIRE(result->my_unordermap.at("b").my_string == "beta");
    }

    SECTION("my_unordermap empty block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_unordermap.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_unordermap.empty());
    }
}
