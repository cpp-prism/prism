#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_vec_enum and my_map_enum all language values round trip", "[json][enum][vector][map]")
{
    SECTION("my_vec_enum with all 4 language values round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_vec_enum = {language::english, language::SimplifiedChinese, language::TraditionalChinese, language::unknow};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_enum.size() == 4);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_vec_enum[1] == language::SimplifiedChinese);
        REQUIRE(result->my_vec_enum[2] == language::TraditionalChinese);
        REQUIRE(result->my_vec_enum[3] == language::unknow);
    }

    SECTION("my_map_enum with unknow value round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map_enum["u"] = language::unknow;
        obj.my_map_enum["e"] = language::english;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map_enum.size() == 2);
        REQUIRE(result->my_map_enum["u"] == language::unknow);
        REQUIRE(result->my_map_enum["e"] == language::english);
    }

    SECTION("lang field + my_vec_enum + my_map_enum all populated round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.lang = language::SimplifiedChinese;
        obj.my_vec_enum = {language::english, language::TraditionalChinese};
        obj.my_map_enum["sc"] = language::SimplifiedChinese;
        obj.my_map_enum["tc"] = language::TraditionalChinese;
        obj.my_map_enum["en"] = language::english;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->lang == language::SimplifiedChinese);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_vec_enum[1] == language::TraditionalChinese);
        REQUIRE(result->my_map_enum.size() == 3);
        REQUIRE(result->my_map_enum["sc"] == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum["tc"] == language::TraditionalChinese);
        REQUIRE(result->my_map_enum["en"] == language::english);
    }
}
