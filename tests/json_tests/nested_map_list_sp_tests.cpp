#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map entries alongside my_list_sp round trip", "[json][map][list][struct][nested]")
{
    SECTION("1 map entry and 2 list_sp elements round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct map_val;
        map_val.my_int = 10;
        map_val.my_list_int.clear();
        map_val.my_list_std_string.clear();
        map_val.my_string = "map_entry_1";
        obj.my_map["mk1"] = std::move(map_val);

        tst_struct lst1;
        lst1.my_int = 20;
        lst1.my_list_int.clear();
        lst1.my_list_std_string.clear();
        lst1.my_string = "list_sp_1";
        obj.my_list_sp.push_back(std::move(lst1));

        tst_struct lst2;
        lst2.my_int = 30;
        lst2.my_list_int.clear();
        lst2.my_list_std_string.clear();
        lst2.my_string = "list_sp_2";
        obj.my_list_sp.push_back(std::move(lst2));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map["mk1"].my_int == 10);
        REQUIRE(result->my_map["mk1"].my_string == "map_entry_1");
        REQUIRE(result->my_list_sp.size() == 2);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_int == 20);
        REQUIRE(it->my_string == "list_sp_1");
        ++it;
        REQUIRE(it->my_int == 30);
    }

    SECTION("map entry with list_sp that has its own my_vec_sp round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct lst;
        lst.my_int = 50;
        lst.my_list_int.clear();
        lst.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 99;
        sub.my_string = "nested_sub";
        lst.my_vec_sp.push_back(sub);

        obj.my_list_sp.push_back(std::move(lst));

        tst_struct map_val;
        map_val.my_int = 60;
        map_val.my_list_int.clear();
        map_val.my_list_std_string.clear();
        obj.my_map["mk2"] = std::move(map_val);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_sp.size() == 1);
        REQUIRE(result->my_list_sp.front().my_int == 50);
        REQUIRE(result->my_list_sp.front().my_vec_sp.size() == 1);
        REQUIRE(result->my_list_sp.front().my_vec_sp[0].my_int == 99);
        REQUIRE(result->my_map["mk2"].my_int == 60);
    }
}
