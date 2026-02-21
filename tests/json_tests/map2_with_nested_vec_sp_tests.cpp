#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map2 entries with nested my_vec_sp children round trip", "[json][map][shared_ptr][vec][nested]")
{
    SECTION("my_map2 entry with nested vec_sp children round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 42;
        sub->my_string = "map2_with_children";

        tst_sub_struct child1;
        child1.my_int = 10;
        child1.my_string = "child1";
        sub->my_vec_sp.push_back(child1);

        tst_sub_struct child2;
        child2.my_int = 20;
        child2.my_opt_int = 99;
        sub->my_vec_sp.push_back(child2);

        obj.my_map2["key_with_children"] = sub;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map2.count("key_with_children") == 1);
        auto& entry = result->my_map2["key_with_children"];
        REQUIRE(entry != nullptr);
        REQUIRE(entry->my_int == 42);
        REQUIRE(entry->my_vec_sp.size() == 2);
        REQUIRE(entry->my_vec_sp[0].my_int == 10);
        REQUIRE(entry->my_vec_sp[0].my_string == "child1");
        REQUIRE(entry->my_vec_sp[1].my_int == 20);
        REQUIRE(entry->my_vec_sp[1].my_opt_int.has_value());
        REQUIRE(*entry->my_vec_sp[1].my_opt_int == 99);
    }

    SECTION("my_map2 2 non-null entries each with different vec_sp round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub1 = std::make_shared<tst_sub_struct>();
        sub1->my_int = 100;
        tst_sub_struct c1;
        c1.my_int = 1;
        sub1->my_vec_sp.push_back(c1);
        obj.my_map2["s1"] = sub1;

        auto sub2 = std::make_shared<tst_sub_struct>();
        sub2->my_int = 200;
        // no vec_sp children
        obj.my_map2["s2"] = sub2;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map2.size() == 2);
        REQUIRE(result->my_map2["s1"]->my_int == 100);
        REQUIRE(result->my_map2["s1"]->my_vec_sp.size() == 1);
        REQUIRE(result->my_map2["s2"]->my_int == 200);
        REQUIRE(result->my_map2["s2"]->my_vec_sp.empty());
    }
}
