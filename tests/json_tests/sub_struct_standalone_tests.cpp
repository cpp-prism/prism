#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - tst_sub_struct standalone serialization round trip", "[json][substruct][standalone]")
{
    SECTION("tst_sub_struct all scalar fields round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 42;
        obj.my_bool = true;
        obj.my_float = 1.5f;
        obj.my_double = 2.75;
        obj.my_string = "standalone";
        obj.my_longlong = 9876543210LL;
        obj.my_opt_str = "opt_standalone";
        obj.my_opt_int = 99;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_int == 42);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_string == "standalone");
        REQUIRE(result->my_longlong == 9876543210LL);
        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "opt_standalone");
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == 99);
    }

    SECTION("tst_sub_struct with my_vec_sp nested element round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 1;
        obj.my_string = "parent";

        tst_sub_struct child;
        child.my_int = 2;
        child.my_string = "child";
        child.my_bool = true;
        obj.my_vec_sp.push_back(child);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_string == "parent");
        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 2);
        REQUIRE(result->my_vec_sp[0].my_string == "child");
        REQUIRE(result->my_vec_sp[0].my_bool == true);
    }

    SECTION("tst_sub_struct default values round trip")
    {
        tst_sub_struct obj;
        // all defaults

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_int == 0);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_string.empty());
        REQUIRE(!result->my_opt_str.has_value());
        REQUIRE(!result->my_opt_int.has_value());
        REQUIRE(result->my_vec_sp.empty());
    }
}
