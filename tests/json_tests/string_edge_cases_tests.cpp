#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - string edge cases round trip", "[json][string]")
{
    SECTION("my_string with embedded newline round trip")
    {
        tst_struct obj;
        obj.my_string = "line1\nline2";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_string == "line1\nline2");
    }

    SECTION("my_string with embedded tab round trip")
    {
        tst_struct obj;
        obj.my_string = "col1\tcol2";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_string == "col1\tcol2");
    }

    SECTION("my_string with backslash round trip")
    {
        tst_struct obj;
        obj.my_string = "path\\to\\file";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_string == "path\\to\\file");
    }

    SECTION("my_string with double quote round trip")
    {
        tst_struct obj;
        obj.my_string = "say \"hello\"";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_string == "say \"hello\"");
    }

    SECTION("my_string all whitespace round trip")
    {
        tst_struct obj;
        obj.my_string = "   ";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_string == "   ");
    }

    SECTION("my_string with forward slash round trip")
    {
        tst_struct obj;
        obj.my_string = "http://example.com/path";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_string == "http://example.com/path");
    }
}
