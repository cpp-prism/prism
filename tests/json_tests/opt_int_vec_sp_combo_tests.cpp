#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_opt_int in sub_struct within vec_sp and standalone round trip", "[json][opt_int][vec_sp][combo]")
{
    SECTION("vec_sp sub_struct with opt_int set round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 7;
        sub.my_opt_int = 42;
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 7);
        REQUIRE(result->my_vec_sp[0].my_opt_int.has_value());
        REQUIRE(*result->my_vec_sp[0].my_opt_int == 42);
    }

    SECTION("vec_sp sub_struct with opt_int null round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 8;
        sub.my_opt_int = std::nullopt;
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(!result->my_vec_sp[0].my_opt_int.has_value());
    }

    SECTION("standalone opt_int with non-null alongside vec_sp round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_int = 999;

        tst_sub_struct sub;
        sub.my_int = 5;
        sub.my_opt_int = -1;
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == 999);
        REQUIRE(result->my_vec_sp[0].my_opt_int.has_value());
        REQUIRE(*result->my_vec_sp[0].my_opt_int == -1);
    }
}
