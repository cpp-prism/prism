#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_set_str large sizes and mixed content round trip", "[json][set][large]")
{
    SECTION("my_set_str six elements round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"apple", "banana", "cherry", "date", "elderberry", "fig"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_set_str.size() == 6);
        REQUIRE(result->my_set_str.count("apple") == 1);
        REQUIRE(result->my_set_str.count("fig") == 1);
    }

    SECTION("my_set_str with single-char strings round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"a", "b", "c", "d"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_set_str.size() == 4);
        REQUIRE(result->my_set_str.count("a") == 1);
        REQUIRE(result->my_set_str.count("d") == 1);
    }

    SECTION("my_set_str with numeric-string entries round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"1", "2", "10", "100"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_set_str.size() == 4);
        REQUIRE(result->my_set_str.count("1") == 1);
        REQUIRE(result->my_set_str.count("100") == 1);
    }
}
