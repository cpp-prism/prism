#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_vec_sp combined with my_deque_int round trip", "[json][vec_sp][deque][combo]")
{
    SECTION("vec_sp two elements and deque five elements round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {1, 2, 3, 4, 5};

        tst_sub_struct s1;
        s1.my_int = 10;
        obj.my_vec_sp.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 20;
        obj.my_vec_sp.push_back(s2);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.size() == 5);
        REQUIRE(result->my_deque_int[4] == 5);
        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 10);
    }

    SECTION("vec_sp empty and deque with negatives round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {-5, -10, -15};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.empty());
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_deque_int[0] == -5);
    }

    SECTION("vec_sp one element and deque single element round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {100};

        tst_sub_struct sub;
        sub.my_int = 77;
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.size() == 1);
        REQUIRE(result->my_deque_int[0] == 100);
        REQUIRE(result->my_vec_sp[0].my_int == 77);
    }
}
