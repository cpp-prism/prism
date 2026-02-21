#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - string escape characters", "[json][string][escape]")
{
    SECTION("newline character round trip")
    {
        std::string original = "line1\nline2";
        std::string json = prism::json::toJsonString(original);
        // 序列化后应包含 \\n 转义
        REQUIRE(json.find("\\n") != std::string::npos);
        auto result = prism::json::fromJsonString<std::string>(json);
        REQUIRE(*result == original);
    }

    SECTION("tab character round trip")
    {
        std::string original = "col1\tcol2";
        std::string json = prism::json::toJsonString(original);
        REQUIRE(json.find("\\t") != std::string::npos);
        auto result = prism::json::fromJsonString<std::string>(json);
        REQUIRE(*result == original);
    }

    SECTION("double quote character round trip")
    {
        std::string original = R"(say "hello")";
        std::string json = prism::json::toJsonString(original);
        REQUIRE(json.find("\\\"") != std::string::npos);
        auto result = prism::json::fromJsonString<std::string>(json);
        REQUIRE(*result == original);
    }

    SECTION("backslash character round trip")
    {
        std::string original = "path\\to\\file";
        std::string json = prism::json::toJsonString(original);
        REQUIRE(json.find("\\\\") != std::string::npos);
        auto result = prism::json::fromJsonString<std::string>(json);
        REQUIRE(*result == original);
    }

    SECTION("carriage return character round trip")
    {
        std::string original = "line1\rline2";
        std::string json = prism::json::toJsonString(original);
        REQUIRE(json.find("\\r") != std::string::npos);
        auto result = prism::json::fromJsonString<std::string>(json);
        REQUIRE(*result == original);
    }

    SECTION("mixed escape characters round trip")
    {
        std::string original = "tab:\there\nnewline and \"quoted\" and back\\slash";
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::string>(json);
        REQUIRE(*result == original);
    }

    SECTION("escape in struct field round trip")
    {
        tst_sub_struct obj;
        obj.my_string = "hello\tworld\n\"end\"";
        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_string == obj.my_string);
    }
}
