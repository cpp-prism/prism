#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_list_sp elements with vec_enum and deque_int round trip", "[json][list_sp][vec_enum][deque][combo]")
{
    SECTION("list_sp element with vec_enum populated round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct entry;
        entry.my_int = 10;
        entry.my_vec_enum = {language::english, language::SimplifiedChinese};
        entry.my_list_int.clear();
        entry.my_list_std_string.clear();
        obj.my_list_sp.push_back(std::move(entry));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_sp.size() == 1);
        REQUIRE(result->my_list_sp.front().my_vec_enum.size() == 2);
        REQUIRE(result->my_list_sp.front().my_vec_enum[0] == language::english);
    }

    SECTION("list_sp element with deque_int populated round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct entry;
        entry.my_int = 20;
        entry.my_deque_int = {5, 10, 15};
        entry.my_list_int.clear();
        entry.my_list_std_string.clear();
        obj.my_list_sp.push_back(std::move(entry));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_sp.front().my_deque_int.size() == 3);
        REQUIRE(result->my_list_sp.front().my_deque_int[0] == 5);
    }

    SECTION("list_sp two elements with different containers round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct e1;
        e1.my_int = 1;
        e1.my_vec_enum = {language::unknow};
        e1.my_list_int.clear();
        e1.my_list_std_string.clear();
        obj.my_list_sp.push_back(std::move(e1));

        tst_struct e2;
        e2.my_int = 2;
        e2.my_deque_int = {100};
        e2.my_list_int.clear();
        e2.my_list_std_string.clear();
        obj.my_list_sp.push_back(std::move(e2));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_sp.size() == 2);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_vec_enum[0] == language::unknow);
        ++it;
        REQUIRE(it->my_deque_int[0] == 100);
    }
}
