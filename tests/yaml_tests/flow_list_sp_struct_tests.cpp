#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_list_sp (list<tst_struct>) round trip", "[yaml][flow][list][struct]")
{
    SECTION("my_list_sp with 2 tst_struct elements flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct elem1;
        elem1.my_int = 10;
        elem1.my_bool = true;
        elem1.my_string = "flow_list_elem1";
        elem1.my_list_int.clear();
        elem1.my_list_std_string.clear();
        obj.my_list_sp.push_back(std::move(elem1));

        tst_struct elem2;
        elem2.my_int = 20;
        elem2.my_bool = false;
        elem2.my_string = "flow_list_elem2";
        elem2.my_list_int.clear();
        elem2.my_list_std_string.clear();
        obj.my_list_sp.push_back(std::move(elem2));

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_list_sp.size() == 2);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_int == 10);
        REQUIRE(it->my_bool == true);
        REQUIRE(it->my_string == "flow_list_elem1");
        ++it;
        REQUIRE(it->my_int == 20);
        REQUIRE(it->my_bool == false);
        REQUIRE(it->my_string == "flow_list_elem2");
    }

    SECTION("my_list_sp empty flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_list_sp.empty());
    }
}
