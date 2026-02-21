#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_struct (embedded tst_sub_struct) nested combo round trip", "[yaml][block][struct][nested][combo]")
{
    SECTION("my_struct with all scalar fields block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_struct.my_int = 42;
        obj.my_struct.my_string = "block_embedded_scalar";
        obj.my_struct.my_opt_int = 7;
        obj.my_struct.my_opt_str = "opt_block_embedded";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_struct.my_int == 42);
        REQUIRE(result->my_struct.my_string == "block_embedded_scalar");
        REQUIRE(result->my_struct.my_opt_int.has_value());
        REQUIRE(*result->my_struct.my_opt_int == 7);
        REQUIRE(result->my_struct.my_opt_str.has_value());
        REQUIRE(*result->my_struct.my_opt_str == "opt_block_embedded");
    }

    SECTION("my_struct with vec_sp alongside root map_enum block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_struct.my_int = 100;
        tst_sub_struct child;
        child.my_int = 55;
        child.my_string = "block_child_of_my_struct";
        obj.my_struct.my_vec_sp.push_back(child);

        obj.my_map_enum["mk1"] = language::english;
        obj.my_map_enum["mk2"] = language::TraditionalChinese;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_struct.my_int == 100);
        REQUIRE(result->my_struct.my_vec_sp.size() == 1);
        REQUIRE(result->my_struct.my_vec_sp[0].my_int == 55);
        REQUIRE(result->my_map_enum["mk1"] == language::english);
        REQUIRE(result->my_map_enum["mk2"] == language::TraditionalChinese);
    }

    SECTION("my_struct nested vec_sp alongside root deque and set block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_struct.my_int = 200;
        obj.my_struct.my_opt_int = 88;
        tst_sub_struct grandchild;
        grandchild.my_int = 444;
        grandchild.my_string = "block_grandchild";
        obj.my_struct.my_vec_sp.push_back(grandchild);

        obj.my_deque_int = {1, 2, 3};
        obj.my_set_str = {"foo", "bar"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_struct.my_int == 200);
        REQUIRE(*result->my_struct.my_opt_int == 88);
        REQUIRE(result->my_struct.my_vec_sp.size() == 1);
        REQUIRE(result->my_struct.my_vec_sp[0].my_int == 444);
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_set_str.count("foo") == 1);
    }
}
