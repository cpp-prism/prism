#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_uptr_sub and my_opt_struct combined with other fields round trip", "[json][unique_ptr][optional][struct][combo]")
{
    SECTION("my_uptr_sub non-null and my_opt_struct with value round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 111;
        obj.my_uptr_sub->my_string = "uptr_val";

        obj.my_opt_struct = tst_sub_struct{};
        obj.my_opt_struct->my_int = 222;
        obj.my_opt_struct->my_string = "opt_val";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 111);
        REQUIRE(result->my_uptr_sub->my_string == "uptr_val");
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 222);
        REQUIRE(result->my_opt_struct->my_string == "opt_val");
    }

    SECTION("my_uptr_sub null and my_opt_struct nullopt round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // uptr_sub defaults to nullptr
        obj.my_opt_struct = std::nullopt;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_uptr_sub == nullptr);
        REQUIRE(!result->my_opt_struct.has_value());
    }

    SECTION("my_uptr_sub with vec_sp alongside my_opt_struct round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 333;

        tst_sub_struct child;
        child.my_int = 444;
        obj.my_uptr_sub->my_vec_sp.push_back(child);

        obj.my_opt_struct = tst_sub_struct{};
        obj.my_opt_struct->my_int = 555;
        obj.my_opt_struct->my_opt_int = 88;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_uptr_sub->my_int == 333);
        REQUIRE(result->my_uptr_sub->my_vec_sp.size() == 1);
        REQUIRE(result->my_uptr_sub->my_vec_sp[0].my_int == 444);
        REQUIRE(result->my_opt_struct->my_int == 555);
        REQUIRE(*result->my_opt_struct->my_opt_int == 88);
    }
}
