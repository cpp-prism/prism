#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map and my_unordermap with special key values round trip", "[json][map][keys]")
{
    SECTION("my_map with single-char keys round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct v1;
        v1.my_int = 10;
        v1.my_list_int.clear();
        v1.my_list_std_string.clear();
        obj.my_map["a"] = std::move(v1);

        tst_struct v2;
        v2.my_int = 20;
        v2.my_list_int.clear();
        v2.my_list_std_string.clear();
        obj.my_map["b"] = std::move(v2);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_map.size() == 2);
        REQUIRE(result->my_map["a"].my_int == 10);
        REQUIRE(result->my_map["b"].my_int == 20);
    }

    SECTION("my_unordermap with numeric-string keys round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct s;
        s.my_int = 100;
        s.my_string = "num_key_val";
        obj.my_unordermap["123"] = s;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_unordermap.size() == 1);
        REQUIRE(result->my_unordermap["123"].my_int == 100);
        REQUIRE(result->my_unordermap["123"].my_string == "num_key_val");
    }

    SECTION("my_map_enum with 3 entries round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map_enum["en"] = language::english;
        obj.my_map_enum["zh_s"] = language::SimplifiedChinese;
        obj.my_map_enum["zh_t"] = language::TraditionalChinese;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_map_enum.size() == 3);
        REQUIRE(result->my_map_enum["en"] == language::english);
        REQUIRE(result->my_map_enum["zh_s"] == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum["zh_t"] == language::TraditionalChinese);
    }
}
