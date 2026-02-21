#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

// Tests for fields added to tst_struct after the initial implementation:
// my_deque_int, my_set_str, my_vec_enum, my_map_enum, my_uptr_sub, my_opt_struct

TEST_CASE("prismJson - newer tst_struct fields comprehensive round trip", "[json][struct][newer_fields]")
{
    SECTION("tst_struct all newer fields populated round trip")
    {
        tst_struct obj;
        obj.my_int = 42;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        // Newer container fields
        obj.my_deque_int = {10, 20, 30};
        obj.my_set_str = {"alpha", "beta", "gamma"};
        obj.my_vec_enum = {english, SimplifiedChinese};
        obj.my_map_enum["en"] = english;
        obj.my_map_enum["zh"] = SimplifiedChinese;

        // Newer pointer/optional fields
        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 77;
        obj.my_uptr_sub->my_string = "uptr_str";

        obj.my_opt_struct = tst_sub_struct{};
        obj.my_opt_struct->my_int = 88;
        obj.my_opt_struct->my_bool = true;
        obj.my_opt_struct->my_string = "opt_struct_str";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 42);

        // Deque
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_deque_int[0] == 10);
        REQUIRE(result->my_deque_int[1] == 20);
        REQUIRE(result->my_deque_int[2] == 30);

        // Set
        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("alpha") == 1);
        REQUIRE(result->my_set_str.count("beta") == 1);
        REQUIRE(result->my_set_str.count("gamma") == 1);

        // Vector of enums
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == english);
        REQUIRE(result->my_vec_enum[1] == SimplifiedChinese);

        // Map of enums
        REQUIRE(result->my_map_enum.size() == 2);
        REQUIRE(result->my_map_enum["en"] == english);
        REQUIRE(result->my_map_enum["zh"] == SimplifiedChinese);

        // unique_ptr<sub>
        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 77);
        REQUIRE(result->my_uptr_sub->my_string == "uptr_str");

        // optional<sub>
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 88);
        REQUIRE(result->my_opt_struct->my_bool == true);
        REQUIRE(result->my_opt_struct->my_string == "opt_struct_str");
    }

    SECTION("tst_struct newer fields all at defaults/null round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // my_deque_int empty by default
        // my_set_str empty by default
        // my_vec_enum empty by default
        // my_map_enum empty by default
        // my_uptr_sub null by default
        // my_opt_struct nullopt by default

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.empty());
        REQUIRE(result->my_set_str.empty());
        REQUIRE(result->my_vec_enum.empty());
        REQUIRE(result->my_map_enum.empty());
        REQUIRE(result->my_uptr_sub == nullptr);
        REQUIRE(!result->my_opt_struct.has_value());
    }
}
