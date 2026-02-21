#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - empty string", "[json][string][empty]")
{
    SECTION("serialize empty string")
    {
        std::string empty = "";
        std::string json = prism::json::toJsonString(empty);
        REQUIRE(json == "\"\"");
    }

    SECTION("deserialize empty string from json")
    {
        std::string json = "\"\"";
        auto result = prism::json::fromJsonString<std::string>(json);
        REQUIRE(*result == "");
    }

    SECTION("round trip empty string")
    {
        std::string original = "";
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::string>(json);
        REQUIRE(*result == original);
    }

    SECTION("serialize optional empty string")
    {
        std::optional<std::string> opt = std::optional<std::string>("");
        std::string json = prism::json::toJsonString(opt);
        REQUIRE(json.find("\"\"") != std::string::npos);
    }
}
