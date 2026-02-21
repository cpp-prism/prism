#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_list_std_string large and varied content round trip", "[json][list_std_string][large]")
{
    SECTION("my_list_std_string eight elements round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        for (int i = 0; i < 8; ++i) {
            obj.my_list_std_string.push_back("item" + std::to_string(i));
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_std_string.size() == 8);
        REQUIRE(result->my_list_std_string.front() == "item0");
        REQUIRE(result->my_list_std_string.back() == "item7");
    }

    SECTION("my_list_std_string with different length strings round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_list_std_string = {"a", "bb", "ccc", "dddd"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_std_string.size() == 4);
        REQUIRE(result->my_list_std_string.front() == "a");
        REQUIRE(result->my_list_std_string.back() == "dddd");
    }

    SECTION("my_list_std_string with numeric strings and empty strings round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_list_std_string = {"100", "200", "300"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_std_string.size() == 3);
        REQUIRE(result->my_list_std_string.front() == "100");
        REQUIRE(result->my_list_std_string.back() == "300");
    }
}
