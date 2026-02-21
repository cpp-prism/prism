#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_vec_enum large sizes and alternating values round trip", "[json][vec_enum][large]")
{
    SECTION("my_vec_enum six elements alternating round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_vec_enum = {
            language::english, language::SimplifiedChinese,
            language::unknow, language::english,
            language::SimplifiedChinese, language::unknow
        };

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_enum.size() == 6);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_vec_enum[2] == language::unknow);
        REQUIRE(result->my_vec_enum[5] == language::unknow);
    }

    SECTION("my_vec_enum all same value round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_vec_enum = {
            language::SimplifiedChinese, language::SimplifiedChinese,
            language::SimplifiedChinese
        };

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_enum.size() == 3);
        for (auto& e : result->my_vec_enum) {
            REQUIRE(e == language::SimplifiedChinese);
        }
    }

    SECTION("my_vec_enum single unknow value round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_vec_enum = {language::unknow};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_enum.size() == 1);
        REQUIRE(result->my_vec_enum[0] == language::unknow);
    }
}
