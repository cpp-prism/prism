#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format list and vec_sp fields round trip", "[yaml][flow][list][vector][struct]")
{
    SECTION("my_list_int with values flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int = {10, 20, 30, 40};
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        auto it = result->my_list_int.begin();
        REQUIRE(*it++ == 10);
        REQUIRE(*it++ == 20);
        REQUIRE(*it++ == 30);
        REQUIRE(*it++ == 40);
    }

    SECTION("my_list_std_string with values flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string = {"alpha", "beta", "gamma"};

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        auto it = result->my_list_std_string.begin();
        REQUIRE(*it++ == "alpha");
        REQUIRE(*it++ == "beta");
        REQUIRE(*it++ == "gamma");
    }

    SECTION("my_vec_sp with 2 elements flow round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub1;
        sub1.my_int = 11;
        sub1.my_bool = true;
        sub1.my_string = "flow_elem1";
        obj.my_vec_sp.push_back(sub1);

        tst_sub_struct sub2;
        sub2.my_int = 22;
        sub2.my_bool = false;
        sub2.my_string = "flow_elem2";
        obj.my_vec_sp.push_back(sub2);

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 11);
        REQUIRE(result->my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_vec_sp[0].my_string == "flow_elem1");
        REQUIRE(result->my_vec_sp[1].my_int == 22);
        REQUIRE(result->my_vec_sp[1].my_bool == false);
        REQUIRE(result->my_vec_sp[1].my_string == "flow_elem2");
    }

    SECTION("my_list_int empty flow round trip")
    {
        tst_struct obj;
        obj.my_int = 4;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 4);
        REQUIRE(result->my_list_int.empty());
    }

    SECTION("my_vec_sp empty flow round trip")
    {
        tst_struct obj;
        obj.my_int = 5;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // my_vec_sp empty by default

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_vec_sp.empty());
    }
}
