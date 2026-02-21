#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map entries whose values have my_vec_enum and lang populated round trip", "[json][map][struct][enum]")
{
    SECTION("my_map entry value with my_vec_enum and lang round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct map_val;
        map_val.my_int = 10;
        map_val.my_list_int.clear();
        map_val.my_list_std_string.clear();
        map_val.lang = language::TraditionalChinese;
        map_val.my_vec_enum = {language::english, language::SimplifiedChinese};
        obj.my_map["key_with_enum"] = std::move(map_val);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map["key_with_enum"].my_int == 10);
        REQUIRE(result->my_map["key_with_enum"].lang == language::TraditionalChinese);
        REQUIRE(result->my_map["key_with_enum"].my_vec_enum.size() == 2);
        REQUIRE(result->my_map["key_with_enum"].my_vec_enum[0] == language::english);
        REQUIRE(result->my_map["key_with_enum"].my_vec_enum[1] == language::SimplifiedChinese);
    }

    SECTION("my_map 2 entries each with different enum state round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct v1;
        v1.my_int = 10;
        v1.my_list_int.clear();
        v1.my_list_std_string.clear();
        v1.lang = language::english;
        v1.my_vec_enum = {language::english};
        obj.my_map["en"] = std::move(v1);

        tst_struct v2;
        v2.my_int = 20;
        v2.my_list_int.clear();
        v2.my_list_std_string.clear();
        v2.lang = language::SimplifiedChinese;
        v2.my_vec_enum = {language::SimplifiedChinese, language::TraditionalChinese};
        obj.my_map["zh"] = std::move(v2);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_map.size() == 2);
        REQUIRE(result->my_map["en"].lang == language::english);
        REQUIRE(result->my_map["en"].my_vec_enum.size() == 1);
        REQUIRE(result->my_map["zh"].lang == language::SimplifiedChinese);
        REQUIRE(result->my_map["zh"].my_vec_enum.size() == 2);
    }
}
