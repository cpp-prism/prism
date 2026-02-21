#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_opt_struct (optional<tst_sub_struct>) full fields round trip", "[json][optional][struct]")
{
    SECTION("my_opt_struct with all scalar fields populated round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 42;
        sub.my_bool = true;
        sub.my_float = 3.14f;
        sub.my_double = 2.718;
        sub.my_string = "opt_full";
        sub.my_longlong = 99999LL;
        sub.my_opt_str = "inner_opt_val";
        sub.my_opt_int = 7;
        obj.my_opt_struct = sub;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 42);
        REQUIRE(result->my_opt_struct->my_bool == true);
        REQUIRE(result->my_opt_struct->my_string == "opt_full");
        REQUIRE(result->my_opt_struct->my_longlong == 99999LL);
        REQUIRE(result->my_opt_struct->my_opt_str.has_value());
        REQUIRE(*result->my_opt_struct->my_opt_str == "inner_opt_val");
        REQUIRE(result->my_opt_struct->my_opt_int.has_value());
        REQUIRE(*result->my_opt_struct->my_opt_int == 7);
    }

    SECTION("my_opt_struct with nested my_vec_sp round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 10;
        sub.my_string = "opt_with_vec";

        tst_sub_struct child;
        child.my_int = 77;
        child.my_string = "vec_child";
        child.my_bool = true;
        sub.my_vec_sp.push_back(child);

        obj.my_opt_struct = sub;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 10);
        REQUIRE(result->my_opt_struct->my_string == "opt_with_vec");
        REQUIRE(result->my_opt_struct->my_vec_sp.size() == 1);
        REQUIRE(result->my_opt_struct->my_vec_sp[0].my_int == 77);
        REQUIRE(result->my_opt_struct->my_vec_sp[0].my_string == "vec_child");
        REQUIRE(result->my_opt_struct->my_vec_sp[0].my_bool == true);
    }
}
