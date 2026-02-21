#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map2 default entries round trip", "[json][map][shared_ptr][default]")
{
    SECTION("my_map2 default entries (key1=non-null, key2=null) survive JSON round trip")
    {
        // Default: {{"key1", make_shared<tst_sub_struct>()}, {"key2", {}}}
        // key2 is nullptr (default-constructed shared_ptr)
        tst_struct obj;
        REQUIRE(obj.my_map2.count("key1") == 1);
        REQUIRE(obj.my_map2["key1"] != nullptr);
        REQUIRE(obj.my_map2.count("key2") == 1);
        REQUIRE(obj.my_map2["key2"] == nullptr);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map2.size() >= 2);
        REQUIRE(result->my_map2.count("key1") == 1);
        REQUIRE(result->my_map2["key1"] != nullptr);
        REQUIRE(result->my_map2["key1"]->my_int == 0);
        REQUIRE(result->my_map2.count("key2") == 1);
        REQUIRE(result->my_map2["key2"] == nullptr);
    }

    SECTION("my_map2 with added entries: all 4 entries survive round trip")
    {
        tst_struct obj;
        // Add entries on top of defaults
        obj.my_map2["extra"] = std::make_shared<tst_sub_struct>();
        obj.my_map2["extra"]->my_int = 77;
        obj.my_map2["extra_null"] = nullptr;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        // key1 (default non-null) must survive
        REQUIRE(result->my_map2.count("key1") == 1);
        REQUIRE(result->my_map2["key1"] != nullptr);
        // key2 (default null) must survive
        REQUIRE(result->my_map2.count("key2") == 1);
        REQUIRE(result->my_map2["key2"] == nullptr);
        // newly added entries
        REQUIRE(result->my_map2.count("extra") == 1);
        REQUIRE(result->my_map2["extra"] != nullptr);
        REQUIRE(result->my_map2["extra"]->my_int == 77);
        REQUIRE(result->my_map2.count("extra_null") == 1);
        REQUIRE(result->my_map2["extra_null"] == nullptr);
    }
}
