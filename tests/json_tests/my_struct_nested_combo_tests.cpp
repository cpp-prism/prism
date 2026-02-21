#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_struct (embedded tst_sub_struct) nested combo round trip", "[json][struct][nested][combo]")
{
    SECTION("my_struct with all scalar fields set round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_struct.my_int = 42;
        obj.my_struct.my_string = "embedded_scalar";
        obj.my_struct.my_opt_int = 7;
        obj.my_struct.my_opt_str = "opt_embedded";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_struct.my_int == 42);
        REQUIRE(result->my_struct.my_string == "embedded_scalar");
        REQUIRE(result->my_struct.my_opt_int.has_value());
        REQUIRE(*result->my_struct.my_opt_int == 7);
        REQUIRE(result->my_struct.my_opt_str.has_value());
        REQUIRE(*result->my_struct.my_opt_str == "opt_embedded");
    }

    SECTION("my_struct with vec_sp and map_enum round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_struct.my_int = 100;
        tst_sub_struct child;
        child.my_int = 55;
        child.my_string = "child_of_my_struct";
        obj.my_struct.my_vec_sp.push_back(child);

        obj.my_map_enum["mk1"] = language::english;
        obj.my_map_enum["mk2"] = language::TraditionalChinese;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_struct.my_int == 100);
        REQUIRE(result->my_struct.my_vec_sp.size() == 1);
        REQUIRE(result->my_struct.my_vec_sp[0].my_int == 55);
        REQUIRE(result->my_map_enum["mk1"] == language::english);
        REQUIRE(result->my_map_enum["mk2"] == language::TraditionalChinese);
    }

    SECTION("my_struct with opt_int and nested vec_sp alongside root deque and set round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_struct.my_int = 200;
        obj.my_struct.my_opt_int = 77;
        tst_sub_struct grandchild;
        grandchild.my_int = 333;
        grandchild.my_string = "grandchild";
        obj.my_struct.my_vec_sp.push_back(grandchild);

        obj.my_deque_int = {5, 10, 15};
        obj.my_set_str = {"alpha", "beta"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_struct.my_int == 200);
        REQUIRE(result->my_struct.my_opt_int.has_value());
        REQUIRE(*result->my_struct.my_opt_int == 77);
        REQUIRE(result->my_struct.my_vec_sp.size() == 1);
        REQUIRE(result->my_struct.my_vec_sp[0].my_int == 333);
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_deque_int[1] == 10);
        REQUIRE(result->my_set_str.count("alpha") == 1);
    }
}
