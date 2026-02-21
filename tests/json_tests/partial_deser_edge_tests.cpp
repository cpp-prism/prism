#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - partial deserialization edge cases round trip", "[json][partial_deser][edge]")
{
    SECTION("JSON missing optional fields uses default values round trip")
    {
        // Build minimal object — only my_int set, optional fields left default
        tst_struct obj;
        obj.my_int = 7;
        obj.my_bool = false;
        obj.my_opt_int = std::nullopt;
        obj.my_opt_str = std::nullopt;
        obj.my_sptr_int.reset();
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        // Serialize then deserialize — optional/nullable fields should remain null/nullopt
        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 7);
        REQUIRE(result->my_bool == false);
        REQUIRE(!result->my_opt_int.has_value());
        REQUIRE(!result->my_opt_str.has_value());
        REQUIRE(result->my_sptr_int == nullptr);
    }

    SECTION("tst_struct serialized then deserialized with only subset verified round trip")
    {
        // Serialize full struct, then only check specific fields round-trip correctly
        tst_struct obj;
        obj.my_int = 5;
        obj.my_bool = true;
        obj.my_opt_int = std::nullopt;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result != nullptr);
        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_bool == true);
        REQUIRE(!result->my_opt_int.has_value());
    }

    SECTION("tst_sub_struct with only my_int set defaults for others")
    {
        tst_sub_struct obj;
        obj.my_int = 99;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_int == 99);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_string == "");
        REQUIRE(!result->my_opt_int.has_value());
    }
}
