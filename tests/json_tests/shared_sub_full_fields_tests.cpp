#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_shared_sub (shared_ptr<tst_sub_struct>) full fields round trip", "[json][shared_ptr][substruct][full]")
{
    SECTION("my_shared_sub with all scalar fields populated round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 88;
        obj.my_shared_sub->my_bool = false;
        obj.my_shared_sub->my_float = 7.5f;
        obj.my_shared_sub->my_double = 9.9;
        obj.my_shared_sub->my_string = "shared_all_fields";
        obj.my_shared_sub->my_longlong = 888888888LL;
        obj.my_shared_sub->my_opt_str = "shared_opt";
        obj.my_shared_sub->my_opt_int = 11;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 88);
        REQUIRE(result->my_shared_sub->my_bool == false);
        REQUIRE(result->my_shared_sub->my_string == "shared_all_fields");
        REQUIRE(result->my_shared_sub->my_longlong == 888888888LL);
        REQUIRE(result->my_shared_sub->my_opt_str.has_value());
        REQUIRE(*result->my_shared_sub->my_opt_str == "shared_opt");
        REQUIRE(result->my_shared_sub->my_opt_int.has_value());
        REQUIRE(*result->my_shared_sub->my_opt_int == 11);
    }

    SECTION("my_shared_sub with 3 my_vec_sp children round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 10;
        obj.my_shared_sub->my_string = "shared_parent_3";

        for (int i = 1; i <= 3; ++i)
        {
            tst_sub_struct child;
            child.my_int = i * 100;
            child.my_bool = (i % 2 == 1);
            child.my_string = "child_" + std::to_string(i);
            obj.my_shared_sub->my_vec_sp.push_back(child);
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 10);
        REQUIRE(result->my_shared_sub->my_vec_sp.size() == 3);
        for (int i = 0; i < 3; ++i)
        {
            REQUIRE(result->my_shared_sub->my_vec_sp[i].my_int == (i + 1) * 100);
            REQUIRE(result->my_shared_sub->my_vec_sp[i].my_bool == ((i + 1) % 2 == 1));
            REQUIRE(result->my_shared_sub->my_vec_sp[i].my_string == "child_" + std::to_string(i + 1));
        }
    }
}
