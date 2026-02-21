#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_vec_enum and my_map_enum combined round trip", "[json][vec][map][enum][combo]")
{
    SECTION("my_vec_enum with multiple enum values round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_vec_enum = {language::english, language::SimplifiedChinese, language::unknow};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_enum.size() == 3);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_vec_enum[1] == language::SimplifiedChinese);
        REQUIRE(result->my_vec_enum[2] == language::unknow);
    }

    SECTION("my_map_enum with multiple enum values round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map_enum["en"] = language::english;
        obj.my_map_enum["zh"] = language::SimplifiedChinese;
        obj.my_map_enum["unk"] = language::unknow;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map_enum.size() == 3);
        REQUIRE(result->my_map_enum.at("en") == language::english);
        REQUIRE(result->my_map_enum.at("zh") == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum.at("unk") == language::unknow);
    }

    SECTION("my_vec_enum and my_map_enum together round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_vec_enum = {language::SimplifiedChinese, language::english};
        obj.my_map_enum["lang1"] = language::SimplifiedChinese;
        obj.my_map_enum["lang2"] = language::unknow;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == language::SimplifiedChinese);
        REQUIRE(result->my_vec_enum[1] == language::english);
        REQUIRE(result->my_map_enum.size() == 2);
        REQUIRE(result->my_map_enum.at("lang1") == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum.at("lang2") == language::unknow);
    }
}
