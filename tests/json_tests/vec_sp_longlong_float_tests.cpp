#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - my_vec_sp elements with longlong and float/double fields round trip", "[json][vec][longlong][float][double]")
{
    SECTION("my_vec_sp element with longlong and small double round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 10;
        sub.my_longlong = 9876543210LL;
        sub.my_float = 0.5f;
        sub.my_double = 0.001;
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_longlong == 9876543210LL);
        REQUIRE(result->my_vec_sp[0].my_float == Catch::Approx(0.5f).epsilon(1e-5));
        REQUIRE(result->my_vec_sp[0].my_double == Catch::Approx(0.001).epsilon(1e-9));
    }

    SECTION("my_vec_sp 2 elements each with different numeric fields round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub1;
        sub1.my_int = 100;
        sub1.my_longlong = 1234567890123LL;
        sub1.my_double = 3.14159;
        obj.my_vec_sp.push_back(sub1);

        tst_sub_struct sub2;
        sub2.my_int = 200;
        sub2.my_longlong = -9876543210LL;
        sub2.my_double = -2.71828;
        obj.my_vec_sp.push_back(sub2);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_longlong == 1234567890123LL);
        REQUIRE(result->my_vec_sp[0].my_double == Catch::Approx(3.14159).epsilon(1e-9));
        REQUIRE(result->my_vec_sp[1].my_longlong == -9876543210LL);
        REQUIRE(result->my_vec_sp[1].my_double == Catch::Approx(-2.71828).epsilon(1e-9));
    }
}
