#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - vec_sp with 7 elements mixed fields round trip", "[json][vec_sp][large][sub_struct]")
{
    SECTION("vec_sp 7 elements each with distinct my_int and bool alternating round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 0; i < 7; ++i) {
            tst_sub_struct sub;
            sub.my_int = i + 1;
            sub.my_bool = (i % 2 == 0);
            obj.my_vec_sp.push_back(sub);
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 7);
        REQUIRE(result->my_vec_sp[0].my_int == 1);
        REQUIRE(result->my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_vec_sp[1].my_bool == false);
        REQUIRE(result->my_vec_sp[6].my_int == 7);
    }

    SECTION("vec_sp 5 elements with string and longlong round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 0; i < 5; ++i) {
            tst_sub_struct sub;
            sub.my_int = i;
            sub.my_string = "item" + std::to_string(i);
            sub.my_longlong = static_cast<long long>(i) * 1000LL;
            obj.my_vec_sp.push_back(sub);
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 5);
        REQUIRE(result->my_vec_sp[0].my_string == "item0");
        REQUIRE(result->my_vec_sp[4].my_string == "item4");
        REQUIRE(result->my_vec_sp[3].my_longlong == 3000LL);
    }
}
