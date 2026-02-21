#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_deque_int and my_set_str with my_vec_enum round trip", "[json][deque][set][vec_enum][combo]")
{
    SECTION("my_deque_int and my_vec_enum together round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {1, 2, 3, 4, 5};
        obj.my_vec_enum = {language::english, language::SimplifiedChinese};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.size() == 5);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_vec_enum[1] == language::SimplifiedChinese);
    }

    SECTION("my_set_str and my_map_enum together round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"cat", "dog", "fish"};
        obj.my_map_enum["e1"] = language::english;
        obj.my_map_enum["e2"] = language::unknow;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("cat") == 1);
        REQUIRE(result->my_map_enum.size() == 2);
        REQUIRE(result->my_map_enum.at("e1") == language::english);
    }

    SECTION("my_deque_int, my_set_str, my_vec_enum all together round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {-10, 0, 10};
        obj.my_set_str = {"one", "two"};
        obj.my_vec_enum = {language::unknow};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_set_str.size() == 2);
        REQUIRE(result->my_vec_enum.size() == 1);
        REQUIRE(result->my_vec_enum[0] == language::unknow);
    }
}
