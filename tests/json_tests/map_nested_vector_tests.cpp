#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map entries with nested my_vec_sp round trip", "[json][map][struct][vector]")
{
    SECTION("my_map entry with 2 my_vec_sp elements round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct map_val;
        map_val.my_int = 100;
        map_val.my_list_int.clear();
        map_val.my_list_std_string.clear();

        tst_sub_struct sub1;
        sub1.my_int = 11;
        sub1.my_bool = true;
        sub1.my_string = "child_one";
        map_val.my_vec_sp.push_back(sub1);

        tst_sub_struct sub2;
        sub2.my_int = 22;
        sub2.my_bool = false;
        sub2.my_string = "child_two";
        map_val.my_vec_sp.push_back(sub2);

        obj.my_map["parent"] = std::move(map_val);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map["parent"].my_int == 100);
        REQUIRE(result->my_map["parent"].my_vec_sp.size() == 2);
        REQUIRE(result->my_map["parent"].my_vec_sp[0].my_int == 11);
        REQUIRE(result->my_map["parent"].my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_map["parent"].my_vec_sp[0].my_string == "child_one");
        REQUIRE(result->my_map["parent"].my_vec_sp[1].my_int == 22);
        REQUIRE(result->my_map["parent"].my_vec_sp[1].my_bool == false);
        REQUIRE(result->my_map["parent"].my_vec_sp[1].my_string == "child_two");
    }

    SECTION("my_map two entries each with 1 my_vec_sp element round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 1; i <= 2; ++i)
        {
            tst_struct map_val;
            map_val.my_int = i * 10;
            map_val.my_list_int.clear();
            map_val.my_list_std_string.clear();

            tst_sub_struct sub;
            sub.my_int = i * 100;
            sub.my_string = "sub_" + std::to_string(i);
            map_val.my_vec_sp.push_back(sub);

            obj.my_map["key" + std::to_string(i)] = std::move(map_val);
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_map.size() == 2);
        for (int i = 1; i <= 2; ++i)
        {
            std::string k = "key" + std::to_string(i);
            REQUIRE(result->my_map[k].my_int == i * 10);
            REQUIRE(result->my_map[k].my_vec_sp.size() == 1);
            REQUIRE(result->my_map[k].my_vec_sp[0].my_int == i * 100);
            REQUIRE(result->my_map[k].my_vec_sp[0].my_string == "sub_" + std::to_string(i));
        }
    }
}
