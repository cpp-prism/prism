#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - tst_struct default list values preserved in round trip", "[json][struct][list][default]")
{
    SECTION("tst_struct my_list_int default {111, 2222} round trip")
    {
        // my_list_int defaults to {111, 2222} — do NOT call .clear()
        tst_struct obj;
        // Verify defaults
        REQUIRE(obj.my_list_int.size() == 2);

        std::string json = prism::json::toJsonString(obj);
        REQUIRE(json.find("\"my_list_int\"") != std::string::npos);
        REQUIRE(json.find("111") != std::string::npos);
        REQUIRE(json.find("2222") != std::string::npos);

        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->my_list_int.size() == 2);
        auto it = result->my_list_int.begin();
        REQUIRE(*it == 111);
        ++it;
        REQUIRE(*it == 2222);
    }

    SECTION("tst_struct my_list_std_string default {\"111\", \"2222\"} round trip")
    {
        tst_struct obj;
        REQUIRE(obj.my_list_std_string.size() == 2);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_std_string.size() == 2);
        auto it = result->my_list_std_string.begin();
        REQUIRE(*it == "111");
        ++it;
        REQUIRE(*it == "2222");
    }

    SECTION("tst_struct my_string default Chinese string round trip")
    {
        // my_string defaults to R"(测试有"号和有 的字符串)"
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        // Default string contains Chinese chars, quote, and space
        REQUIRE(result->my_string == obj.my_string);
        REQUIRE(result->my_string.find("测试") != std::string::npos);
    }
}
