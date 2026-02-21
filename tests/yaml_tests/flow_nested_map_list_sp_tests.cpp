#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_map entries alongside my_list_sp round trip", "[yaml][flow][map][list][struct][nested]")
{
    SECTION("1 map entry and 2 list_sp elements flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct map_val;
        map_val.my_int = 10;
        map_val.my_list_int.clear();
        map_val.my_list_std_string.clear();
        map_val.my_string = "flow_map_entry_1";
        obj.my_map["mk1"] = std::move(map_val);

        tst_struct lst1;
        lst1.my_int = 20;
        lst1.my_list_int.clear();
        lst1.my_list_std_string.clear();
        obj.my_list_sp.push_back(std::move(lst1));

        tst_struct lst2;
        lst2.my_int = 30;
        lst2.my_list_int.clear();
        lst2.my_list_std_string.clear();
        obj.my_list_sp.push_back(std::move(lst2));

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map["mk1"].my_int == 10);
        REQUIRE(result->my_list_sp.size() == 2);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_int == 20);
        ++it;
        REQUIRE(it->my_int == 30);
    }

    SECTION("map entry with vec_enum and list_sp with nested vec_sp flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct map_val;
        map_val.my_int = 15;
        map_val.my_list_int.clear();
        map_val.my_list_std_string.clear();
        map_val.my_vec_enum = {language::english};
        obj.my_map["mk2"] = std::move(map_val);

        tst_struct lst;
        lst.my_int = 25;
        lst.my_list_int.clear();
        lst.my_list_std_string.clear();
        tst_sub_struct sub;
        sub.my_int = 222;
        sub.my_string = "flow_nested_sub_in_list";
        lst.my_vec_sp.push_back(sub);
        obj.my_list_sp.push_back(std::move(lst));

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map["mk2"].my_vec_enum.size() == 1);
        REQUIRE(result->my_map["mk2"].my_vec_enum[0] == language::english);
        REQUIRE(result->my_list_sp.front().my_int == 25);
        REQUIRE(result->my_list_sp.front().my_vec_sp.size() == 1);
        REQUIRE(result->my_list_sp.front().my_vec_sp[0].my_int == 222);
    }
}
