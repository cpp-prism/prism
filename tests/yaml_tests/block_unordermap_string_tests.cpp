#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_unordermap with various tst_sub_struct values round trip", "[yaml][block][unordermap][string][combo]")
{
    SECTION("my_unordermap single entry block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_unordermap.clear();

        tst_sub_struct sub;
        sub.my_int = 10;
        sub.my_longlong = 100LL;
        obj.my_unordermap["entry1"] = sub;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_unordermap.size() == 1);
        REQUIRE(result->my_unordermap.at("entry1").my_int == 10);
        REQUIRE(result->my_unordermap.at("entry1").my_longlong == 100LL);
    }

    SECTION("my_unordermap multiple entries block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_unordermap.clear();

        tst_sub_struct s1;
        s1.my_int = 11;
        s1.my_longlong = 111LL;
        obj.my_unordermap["key1"] = s1;

        tst_sub_struct s2;
        s2.my_int = 22;
        s2.my_longlong = 222LL;
        obj.my_unordermap["key2"] = s2;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_unordermap.size() == 2);
        REQUIRE(result->my_unordermap.at("key1").my_int == 11);
        REQUIRE(result->my_unordermap.at("key1").my_longlong == 111LL);
        REQUIRE(result->my_unordermap.at("key2").my_int == 22);
        REQUIRE(result->my_unordermap.at("key2").my_longlong == 222LL);
    }
}
