#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - optional<string> edge cases in struct", "[json][optional][string]")
{
    SECTION("tst_struct my_opt_str nullopt serializes as null")
    {
        tst_struct obj;
        // my_opt_str defaults to nullopt

        std::string json = prism::json::toJsonString(obj);
        // must contain "my_opt_str":null
        REQUIRE(json.find("\"my_opt_str\":null") != std::string::npos);

        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(!result->my_opt_str.has_value());
    }

    SECTION("tst_struct my_opt_str = empty string round trip (distinct from null)")
    {
        tst_struct obj;
        obj.my_opt_str = "";

        std::string json = prism::json::toJsonString(obj);
        // must contain "my_opt_str":"" (empty string, not null)
        REQUIRE(json.find("\"my_opt_str\":\"\"") != std::string::npos);

        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "");
    }

    SECTION("tst_sub_struct my_opt_str = null-looking string distinct from nullopt")
    {
        tst_sub_struct obj;
        obj.my_int = 5;
        obj.my_opt_str = "null";  // the string "null", not nullopt

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_opt_int.has_value() == false);  // my_opt_int not set
        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "null");
    }

    SECTION("tst_sub_struct my_opt_int = 0 distinct from null in JSON round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 6;
        obj.my_opt_int = 0;

        std::string json = prism::json::toJsonString(obj);
        // must contain "my_opt_int":0 (not null)
        REQUIRE(json.find("\"my_opt_int\":0") != std::string::npos);

        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == 0);
    }
}
