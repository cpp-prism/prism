#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map_enum with multiple entries round trip", "[json][map_enum][large]")
{
    SECTION("my_map_enum three entries all different values round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map_enum["k1"] = language::english;
        obj.my_map_enum["k2"] = language::SimplifiedChinese;
        obj.my_map_enum["k3"] = language::unknow;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map_enum.size() == 3);
        REQUIRE(result->my_map_enum.at("k1") == language::english);
        REQUIRE(result->my_map_enum.at("k2") == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum.at("k3") == language::unknow);
    }

    SECTION("my_map_enum with same value for different keys round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map_enum["a"] = language::english;
        obj.my_map_enum["b"] = language::english;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map_enum.size() == 2);
        REQUIRE(result->my_map_enum.at("a") == language::english);
        REQUIRE(result->my_map_enum.at("b") == language::english);
    }

    SECTION("my_map_enum empty round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map_enum.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map_enum.empty());
    }
}
