#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - my_vec_sp elements with multiple fields set simultaneously round trip", "[json][vec][multi][fields]")
{
    SECTION("my_vec_sp element with all common fields set round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 42;
        sub.my_bool = true;
        sub.my_float = 1.5f;
        sub.my_double = 2.5;
        sub.my_string = "multi_field";
        sub.my_longlong = 9876LL;
        sub.my_opt_str = "opt_val";
        sub.my_opt_int = 99;
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 1);
        auto& r = result->my_vec_sp[0];
        REQUIRE(r.my_int == 42);
        REQUIRE(r.my_bool == true);
        REQUIRE(r.my_float == Catch::Approx(1.5f).epsilon(1e-4));
        REQUIRE(r.my_double == Catch::Approx(2.5).epsilon(1e-9));
        REQUIRE(r.my_string == "multi_field");
        REQUIRE(r.my_longlong == 9876LL);
        REQUIRE(r.my_opt_str.has_value());
        REQUIRE(r.my_opt_str.value() == "opt_val");
        REQUIRE(r.my_opt_int.has_value());
        REQUIRE(r.my_opt_int.value() == 99);
    }

    SECTION("my_vec_sp 3 elements each with distinct fields round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 0; i < 3; ++i) {
            tst_sub_struct s;
            s.my_int = i * 10;
            s.my_longlong = static_cast<long long>(i * 100);
            s.my_bool = (i % 2 == 0);
            obj.my_vec_sp.push_back(s);
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 3);
        for (int i = 0; i < 3; ++i) {
            REQUIRE(result->my_vec_sp[i].my_int == i * 10);
            REQUIRE(result->my_vec_sp[i].my_longlong == static_cast<long long>(i * 100));
            REQUIRE(result->my_vec_sp[i].my_bool == (i % 2 == 0));
        }
    }
}
