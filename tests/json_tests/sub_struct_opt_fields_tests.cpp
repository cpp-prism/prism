#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - tst_sub_struct optional fields with nested vec_sp round trip", "[json][struct][optional][vec]")
{
    SECTION("tst_sub_struct with all optional fields set round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 42;
        sub.my_bool = true;
        sub.my_float = 1.5f;
        sub.my_double = 3.14;
        sub.my_longlong = 12345678LL;
        sub.my_string = "full_sub";
        sub.my_opt_int = 99;
        sub.my_opt_str = "opt_sub_str";
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 1);
        auto& s = result->my_vec_sp[0];
        REQUIRE(s.my_int == 42);
        REQUIRE(s.my_bool == true);
        REQUIRE(s.my_float == Catch::Approx(1.5f).epsilon(1e-5));
        REQUIRE(s.my_double == Catch::Approx(3.14).epsilon(1e-9));
        REQUIRE(s.my_longlong == 12345678LL);
        REQUIRE(s.my_string == "full_sub");
        REQUIRE(s.my_opt_int.has_value());
        REQUIRE(*s.my_opt_int == 99);
        REQUIRE(s.my_opt_str.has_value());
        REQUIRE(*s.my_opt_str == "opt_sub_str");
    }

    SECTION("tst_sub_struct with opt fields null round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 10;
        sub.my_opt_int = std::nullopt;
        sub.my_opt_str = std::nullopt;
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(!result->my_vec_sp[0].my_opt_int.has_value());
        REQUIRE(!result->my_vec_sp[0].my_opt_str.has_value());
    }

    SECTION("tst_sub_struct with nested vec_sp children and their opts round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct parent;
        parent.my_int = 100;
        parent.my_opt_str = "parent_opt";

        tst_sub_struct child;
        child.my_int = 200;
        child.my_opt_int = 77;
        parent.my_vec_sp.push_back(child);

        obj.my_vec_sp.push_back(parent);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 100);
        REQUIRE(*result->my_vec_sp[0].my_opt_str == "parent_opt");
        REQUIRE(result->my_vec_sp[0].my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_vec_sp[0].my_int == 200);
        REQUIRE(*result->my_vec_sp[0].my_vec_sp[0].my_opt_int == 77);
    }
}
