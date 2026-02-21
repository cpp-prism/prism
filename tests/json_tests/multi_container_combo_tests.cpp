#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - multiple containers (deque, set, vec_enum, list_int) together round trip", "[json][deque][set][vec_enum][list_int][combo]")
{
    SECTION("deque, set, vec_enum, list_int all populated round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_deque_int = {1, 2, 3};
        obj.my_set_str = {"x", "y", "z"};
        obj.my_vec_enum = {language::english, language::unknow};
        obj.my_list_int = {10, 20};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("y") == 1);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_list_int.size() == 2);
    }

    SECTION("deque_int and set_str both empty, vec_enum single value round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int.clear();
        obj.my_set_str.clear();
        obj.my_vec_enum = {language::SimplifiedChinese};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.empty());
        REQUIRE(result->my_set_str.empty());
        REQUIRE(result->my_vec_enum.size() == 1);
        REQUIRE(result->my_vec_enum[0] == language::SimplifiedChinese);
    }

    SECTION("all containers empty round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int.clear();
        obj.my_set_str.clear();
        obj.my_vec_enum.clear();
        obj.my_map_enum.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.empty());
        REQUIRE(result->my_set_str.empty());
        REQUIRE(result->my_vec_enum.empty());
        REQUIRE(result->my_map_enum.empty());
    }
}
