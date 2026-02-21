#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - recursive shared_ptr<tst_struct> chain round trip", "[json][struct][shared_ptr][recursive]")
{
    SECTION("tst_struct my_shared_self 2-level chain round trip")
    {
        tst_struct outer;
        outer.my_int = 1;
        outer.my_list_int.clear();
        outer.my_list_std_string.clear();

        outer.my_shared_self = std::make_shared<tst_struct>();
        outer.my_shared_self->my_int = 2;
        outer.my_shared_self->my_string = "level_2";
        outer.my_shared_self->my_list_int.clear();
        outer.my_shared_self->my_list_std_string.clear();

        // Level 3 leaf: shared_self is null (terminates recursion)
        outer.my_shared_self->my_shared_self = std::make_shared<tst_struct>();
        outer.my_shared_self->my_shared_self->my_int = 3;
        outer.my_shared_self->my_shared_self->my_string = "level_3";
        outer.my_shared_self->my_shared_self->my_list_int.clear();
        outer.my_shared_self->my_shared_self->my_list_std_string.clear();

        std::string json = prism::json::toJsonString(outer);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 2);
        REQUIRE(result->my_shared_self->my_string == "level_2");
        REQUIRE(result->my_shared_self->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_shared_self->my_int == 3);
        REQUIRE(result->my_shared_self->my_shared_self->my_string == "level_3");
        REQUIRE(result->my_shared_self->my_shared_self->my_shared_self == nullptr);
    }

    SECTION("tst_struct my_shared_self with my_vec_sp in child node round trip")
    {
        tst_struct outer;
        outer.my_int = 10;
        outer.my_list_int.clear();
        outer.my_list_std_string.clear();

        outer.my_shared_self = std::make_shared<tst_struct>();
        outer.my_shared_self->my_int = 20;
        outer.my_shared_self->my_list_int.clear();
        outer.my_shared_self->my_list_std_string.clear();

        tst_sub_struct child_sub;
        child_sub.my_int = 99;
        child_sub.my_string = "child_sub_str";
        outer.my_shared_self->my_vec_sp.push_back(child_sub);

        std::string json = prism::json::toJsonString(outer);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 10);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 20);
        REQUIRE(result->my_shared_self->my_vec_sp.size() == 1);
        REQUIRE(result->my_shared_self->my_vec_sp[0].my_int == 99);
        REQUIRE(result->my_shared_self->my_vec_sp[0].my_string == "child_sub_str");
    }
}
