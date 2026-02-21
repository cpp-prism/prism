#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_list_sp (list<tst_struct>) with optional fields round trip", "[yaml][block][list_sp][optional][fields]")
{
    SECTION("my_list_sp element with opt_str populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct sub;
        sub.my_int = 10;
        sub.my_list_int.clear();
        sub.my_list_std_string.clear();
        sub.my_opt_str = "list_sp_opt";
        obj.my_list_sp.push_back(std::move(sub));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_sp.size() == 1);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_int == 10);
        REQUIRE(it->my_opt_str.has_value());
        REQUIRE(it->my_opt_str.value() == "list_sp_opt");
    }

    SECTION("my_list_sp two elements with bool fields block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct s1;
        s1.my_int = 1;
        s1.my_list_int.clear();
        s1.my_list_std_string.clear();
        s1.my_bool = true;
        obj.my_list_sp.push_back(std::move(s1));

        tst_struct s2;
        s2.my_int = 2;
        s2.my_list_int.clear();
        s2.my_list_std_string.clear();
        s2.my_bool = false;
        obj.my_list_sp.push_back(std::move(s2));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_sp.size() == 2);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_bool == true); ++it;
        REQUIRE(it->my_bool == false);
    }
}
