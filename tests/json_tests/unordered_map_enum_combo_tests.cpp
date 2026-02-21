#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_unordermap with enum-bearing values round trip", "[json][unordered_map][struct][enum]")
{
    SECTION("my_unordermap entry with my_vec_enum round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct val;
        val.my_int = 55;
        val.my_string = "umap_enum_val";
        obj.my_unordermap["key_a"] = val;

        obj.my_vec_enum = {language::english, language::TraditionalChinese};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_unordermap.size() == 1);
        REQUIRE(result->my_unordermap["key_a"].my_int == 55);
        REQUIRE(result->my_unordermap["key_a"].my_string == "umap_enum_val");
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_vec_enum[1] == language::TraditionalChinese);
    }

    SECTION("my_unordermap 2 entries with lang and map_enum round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct val1;
        val1.my_int = 10;
        val1.my_bool = true;
        obj.my_unordermap["u1"] = val1;

        tst_sub_struct val2;
        val2.my_int = 20;
        val2.my_string = "second_umap";
        obj.my_unordermap["u2"] = val2;

        obj.lang = language::TraditionalChinese;
        obj.my_map_enum["lang_a"] = language::SimplifiedChinese;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_unordermap.size() == 2);
        REQUIRE(result->my_unordermap["u1"].my_int == 10);
        REQUIRE(result->my_unordermap["u1"].my_bool == true);
        REQUIRE(result->my_unordermap["u2"].my_int == 20);
        REQUIRE(result->lang == language::TraditionalChinese);
        REQUIRE(result->my_map_enum["lang_a"] == language::SimplifiedChinese);
    }
}
