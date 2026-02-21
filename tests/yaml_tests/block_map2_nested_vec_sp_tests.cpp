#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map2 entries with nested my_vec_sp children round trip", "[yaml][block][map][shared_ptr][vec][nested]")
{
    SECTION("my_map2 entry with nested vec_sp children block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 42;
        sub->my_string = "block_map2_children";

        tst_sub_struct child1;
        child1.my_int = 10;
        child1.my_string = "block_child1";
        sub->my_vec_sp.push_back(child1);

        tst_sub_struct child2;
        child2.my_int = 20;
        child2.my_opt_int = 77;
        sub->my_vec_sp.push_back(child2);

        obj.my_map2["bkey_children"] = sub;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map2.count("bkey_children") == 1);
        auto& entry = result->my_map2["bkey_children"];
        REQUIRE(entry != nullptr);
        REQUIRE(entry->my_int == 42);
        REQUIRE(entry->my_vec_sp.size() == 2);
        REQUIRE(entry->my_vec_sp[0].my_int == 10);
        REQUIRE(entry->my_vec_sp[1].my_int == 20);
        REQUIRE(*entry->my_vec_sp[1].my_opt_int == 77);
    }
}
