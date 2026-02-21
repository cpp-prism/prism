#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format list<struct> as struct field", "[yaml][block][list][struct]")
{
    SECTION("tst_struct my_list_sp two elements block round trip")
    {
        tst_struct obj;
        obj.my_int = 9;

        tst_struct inner1;
        inner1.my_int = 111;
        inner1.my_bool = true;
        obj.my_list_sp.push_back(std::move(inner1));

        tst_struct inner2;
        inner2.my_int = 222;
        inner2.my_bool = false;
        obj.my_list_sp.push_back(std::move(inner2));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 9);
        REQUIRE(result->my_list_sp.size() == 2);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_int == 111);
        REQUIRE(it->my_bool == true);
        ++it;
        REQUIRE(it->my_int == 222);
        REQUIRE(it->my_bool == false);
    }

    SECTION("tst_struct my_list_int non-empty block round trip")
    {
        tst_struct obj;
        obj.my_list_int = {5, 10, 15};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_int.size() == 3);
        auto it = result->my_list_int.begin();
        REQUIRE(*it++ == 5);
        REQUIRE(*it++ == 10);
        REQUIRE(*it++ == 15);
    }

    SECTION("tst_struct my_list_std_string non-empty block round trip")
    {
        tst_struct obj;
        obj.my_list_std_string = {"alpha", "beta", "gamma"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_std_string.size() == 3);
        auto it = result->my_list_std_string.begin();
        REQUIRE(*it++ == "alpha");
        REQUIRE(*it++ == "beta");
        REQUIRE(*it++ == "gamma");
    }
}
