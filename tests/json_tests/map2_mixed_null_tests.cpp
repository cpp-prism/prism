#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map2 mixed null and non-null entries round trip", "[json][map][shared_ptr][null]")
{
    SECTION("my_map2 single non-null entry round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 42;
        sub->my_string = "map2_non_null";
        obj.my_map2["key_a"] = sub;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map2.count("key_a") == 1);
        REQUIRE(result->my_map2["key_a"] != nullptr);
        REQUIRE(result->my_map2["key_a"]->my_int == 42);
        REQUIRE(result->my_map2["key_a"]->my_string == "map2_non_null");
    }

    SECTION("my_map2 null entry round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        obj.my_map2["null_key"] = nullptr;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map2.count("null_key") == 1);
        REQUIRE(result->my_map2["null_key"] == nullptr);
    }

    SECTION("my_map2 mixed null and non-null entries round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 77;
        obj.my_map2["active"] = sub;
        obj.my_map2["inactive"] = nullptr;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map2.size() == 2);
        REQUIRE(result->my_map2["active"] != nullptr);
        REQUIRE(result->my_map2["active"]->my_int == 77);
        REQUIRE(result->my_map2["inactive"] == nullptr);
    }
}
