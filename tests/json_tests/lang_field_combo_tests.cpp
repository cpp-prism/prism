#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - tst_struct lang field combined with other fields round trip", "[json][lang][enum][combo]")
{
    SECTION("lang field with various enum values round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.lang = language::english;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->lang == language::english);
    }

    SECTION("lang field SimplifiedChinese with list round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int = {10, 20};
        obj.my_list_std_string.clear();
        obj.lang = language::SimplifiedChinese;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->lang == language::SimplifiedChinese);
        REQUIRE(result->my_list_int.size() == 2);
    }

    SECTION("lang field unknow with vec_enum round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.lang = language::unknow;
        obj.my_vec_enum = {language::english, language::SimplifiedChinese};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->lang == language::unknow);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_vec_enum[1] == language::SimplifiedChinese);
    }
}
