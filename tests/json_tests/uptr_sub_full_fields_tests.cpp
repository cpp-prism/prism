#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_uptr_sub (unique_ptr<tst_sub_struct>) full fields round trip", "[json][unique_ptr][substruct]")
{
    SECTION("my_uptr_sub with all scalar fields populated round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 88;
        obj.my_uptr_sub->my_bool = false;
        obj.my_uptr_sub->my_float = 1.1f;
        obj.my_uptr_sub->my_double = 2.2;
        obj.my_uptr_sub->my_string = "uptr_all_fields";
        obj.my_uptr_sub->my_longlong = 654321LL;
        obj.my_uptr_sub->my_opt_str = "opt_in_uptr";
        obj.my_uptr_sub->my_opt_int = 33;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 88);
        REQUIRE(result->my_uptr_sub->my_bool == false);
        REQUIRE(result->my_uptr_sub->my_string == "uptr_all_fields");
        REQUIRE(result->my_uptr_sub->my_longlong == 654321LL);
        REQUIRE(result->my_uptr_sub->my_opt_str.has_value());
        REQUIRE(*result->my_uptr_sub->my_opt_str == "opt_in_uptr");
        REQUIRE(result->my_uptr_sub->my_opt_int.has_value());
        REQUIRE(*result->my_uptr_sub->my_opt_int == 33);
    }

    SECTION("my_uptr_sub with my_vec_sp nested round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 5;
        obj.my_uptr_sub->my_string = "uptr_with_vec_json";

        for (int i = 1; i <= 2; ++i)
        {
            tst_sub_struct child;
            child.my_int = i * 100;
            child.my_string = "json_uptr_child_" + std::to_string(i);
            obj.my_uptr_sub->my_vec_sp.push_back(child);
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 5);
        REQUIRE(result->my_uptr_sub->my_vec_sp.size() == 2);
        for (int i = 0; i < 2; ++i)
        {
            REQUIRE(result->my_uptr_sub->my_vec_sp[i].my_int == (i + 1) * 100);
            REQUIRE(result->my_uptr_sub->my_vec_sp[i].my_string == "json_uptr_child_" + std::to_string(i + 1));
        }
    }
}
