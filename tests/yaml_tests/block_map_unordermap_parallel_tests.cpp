#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map and my_unordermap populated simultaneously round trip", "[yaml][block][map][unordered_map][struct]")
{
    SECTION("my_map and my_unordermap each with 1 entry block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct map_val;
        map_val.my_int = 10;
        map_val.my_list_int.clear();
        map_val.my_list_std_string.clear();
        map_val.my_string = "block_map_entry";
        obj.my_map["mk"] = std::move(map_val);

        tst_sub_struct umap_val;
        umap_val.my_int = 20;
        umap_val.my_string = "block_umap_entry";
        obj.my_unordermap["uk"] = umap_val;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map["mk"].my_int == 10);
        REQUIRE(result->my_map["mk"].my_string == "block_map_entry");
        REQUIRE(result->my_unordermap.size() == 1);
        REQUIRE(result->my_unordermap["uk"].my_int == 20);
        REQUIRE(result->my_unordermap["uk"].my_string == "block_umap_entry");
    }

    SECTION("my_map 2 entries and my_unordermap 2 entries with enum block round trip")
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
            obj.my_map["bmk" + std::to_string(i)] = std::move(mv);
        }

        for (int i = 1; i <= 2; ++i)
        {
            tst_sub_struct uv;
            uv.my_int = i * 100;
            uv.my_bool = (i % 2 == 0);
            obj.my_unordermap["buk" + std::to_string(i)] = uv;
        }

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.size() == 2);
        REQUIRE(result->my_map["bmk1"].lang == language::english);
        REQUIRE(result->my_map["bmk2"].lang == language::SimplifiedChinese);
        REQUIRE(result->my_unordermap.size() == 2);
        REQUIRE(result->my_unordermap["buk1"].my_int == 100);
        REQUIRE(result->my_unordermap["buk2"].my_int == 200);
        REQUIRE(result->my_unordermap["buk2"].my_bool == true);
    }
}
