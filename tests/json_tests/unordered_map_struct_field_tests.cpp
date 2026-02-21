#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - tst_struct my_unordermap (unordered_map<string,struct>) field round trip", "[json][struct][unordered_map]")
{
    SECTION("tst_struct my_unordermap two entries round trip")
    {
        tst_struct obj;
        obj.my_int = 8;

        tst_sub_struct s1;
        s1.my_int = 10;
        s1.my_bool = true;
        s1.my_string = "alpha_val";
        obj.my_unordermap["alpha"] = s1;

        tst_sub_struct s2;
        s2.my_int = 20;
        s2.my_bool = false;
        s2.my_string = "beta_val";
        obj.my_unordermap["beta"] = s2;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 8);
        REQUIRE(result->my_unordermap.size() == 2);
        REQUIRE(result->my_unordermap["alpha"].my_int == 10);
        REQUIRE(result->my_unordermap["alpha"].my_bool == true);
        REQUIRE(result->my_unordermap["alpha"].my_string == "alpha_val");
        REQUIRE(result->my_unordermap["beta"].my_int == 20);
        REQUIRE(result->my_unordermap["beta"].my_bool == false);
        REQUIRE(result->my_unordermap["beta"].my_string == "beta_val");
    }

    SECTION("tst_struct my_unordermap empty round trip")
    {
        tst_struct obj;
        obj.my_int = 4;
        // my_unordermap is empty by default

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 4);
        REQUIRE(result->my_unordermap.empty());
    }
}
