#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_vec_sp combined with my_deque_int and my_set_str round trip", "[json][vec][deque][set][struct]")
{
    SECTION("my_vec_sp 2 elements with my_deque_int and my_set_str round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub1;
        sub1.my_int = 10;
        sub1.my_string = "sub1_deque_set";
        obj.my_vec_sp.push_back(sub1);

        tst_sub_struct sub2;
        sub2.my_int = 20;
        sub2.my_bool = true;
        obj.my_vec_sp.push_back(sub2);

        obj.my_deque_int = {1, 2, 3, 4, 5};
        obj.my_set_str = {"alpha", "beta", "gamma"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 10);
        REQUIRE(result->my_vec_sp[1].my_int == 20);
        REQUIRE(result->my_deque_int.size() == 5);
        REQUIRE(result->my_deque_int[4] == 5);
        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("gamma") == 1);
    }

    SECTION("empty vec_sp with populated deque and set round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // no vec_sp
        obj.my_deque_int = {-10, 0, 10};
        obj.my_set_str = {"only_one"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.empty());
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_deque_int[0] == -10);
        REQUIRE(result->my_set_str.size() == 1);
        REQUIRE(result->my_set_str.count("only_one") == 1);
    }

    SECTION("vec_sp with sub element and its opt fields alongside deque round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 50;
        sub.my_opt_int = 99;
        sub.my_opt_str = "sub_opt_str";
        obj.my_vec_sp.push_back(sub);

        obj.my_deque_int = {100, 200};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_opt_int.has_value());
        REQUIRE(*result->my_vec_sp[0].my_opt_int == 99);
        REQUIRE(result->my_vec_sp[0].my_opt_str.has_value());
        REQUIRE(*result->my_vec_sp[0].my_opt_str == "sub_opt_str");
        REQUIRE(result->my_deque_int.size() == 2);
    }
}
