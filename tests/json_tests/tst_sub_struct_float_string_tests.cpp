#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - tst_sub_struct float and string field combinations round trip", "[json][sub_struct][float][string]")
{
    SECTION("tst_sub_struct float positive and non-empty string round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 1;
        obj.my_float = 2.5f;
        obj.my_string = "hello_sub";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_float == Catch::Approx(2.5f));
        REQUIRE(result->my_string == "hello_sub");
    }

    SECTION("tst_sub_struct float negative and empty string round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 2;
        obj.my_float = -0.75f;
        obj.my_string = "";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_float == Catch::Approx(-0.75f));
        REQUIRE(result->my_string == "");
    }

    SECTION("tst_sub_struct float zero and string with digits round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 3;
        obj.my_float = 0.0f;
        obj.my_string = "007";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);

        REQUIRE(result->my_float == 0.0f);
        REQUIRE(result->my_string == "007");
    }

    SECTION("tst_sub_struct in vec_sp with float and string round trip")
    {
        tst_struct obj;
        obj.my_int = 4;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 5;
        sub.my_float = 1.234f;
        sub.my_string = "in_vec";
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp[0].my_float == Catch::Approx(1.234f).epsilon(0.001));
        REQUIRE(result->my_vec_sp[0].my_string == "in_vec");
    }
}
