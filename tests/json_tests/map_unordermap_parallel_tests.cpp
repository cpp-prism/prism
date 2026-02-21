#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map and my_unordermap populated simultaneously round trip", "[json][map][unordered_map][struct]")
{
    SECTION("my_map and my_unordermap each with 1 entry round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct map_val;
        map_val.my_int = 10;
        map_val.my_list_int.clear();
        map_val.my_list_std_string.clear();
        map_val.my_string = "map_entry";
        obj.my_map["mk"] = std::move(map_val);

        tst_sub_struct umap_val;
        umap_val.my_int = 20;
        umap_val.my_string = "umap_entry";
        obj.my_unordermap["uk"] = umap_val;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map["mk"].my_int == 10);
        REQUIRE(result->my_map["mk"].my_string == "map_entry");
        REQUIRE(result->my_unordermap.size() == 1);
        REQUIRE(result->my_unordermap["uk"].my_int == 20);
        REQUIRE(result->my_unordermap["uk"].my_string == "umap_entry");
    }

    SECTION("my_map 2 entries and my_unordermap 2 entries round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 1; i <= 2; ++i)
        {
            tst_struct mv;
            mv.my_int = i * 10;
            mv.my_list_int.clear();
            mv.my_list_std_string.clear();
            mv.lang = (i == 1) ? language::english : language::SimplifiedChinese;
            obj.my_map["mk" + std::to_string(i)] = std::move(mv);
        }

        for (int i = 1; i <= 2; ++i)
        {
            tst_sub_struct uv;
            uv.my_int = i * 100;
            uv.my_string = "uv_" + std::to_string(i);
            obj.my_unordermap["uk" + std::to_string(i)] = uv;
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.size() == 2);
        REQUIRE(result->my_map["mk1"].lang == language::english);
        REQUIRE(result->my_map["mk2"].lang == language::SimplifiedChinese);
        REQUIRE(result->my_unordermap.size() == 2);
        REQUIRE(result->my_unordermap["uk1"].my_int == 100);
        REQUIRE(result->my_unordermap["uk2"].my_int == 200);
    }
}
