#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format deeply nested arrays", "[yaml][block][nested][array]")
{
    SECTION("3-level recursive tst_sub_struct my_vec_sp block round trip")
    {
        // outer -> inner -> deepest (3 levels of my_vec_sp nesting)
        tst_sub_struct deepest;
        deepest.my_int = 999;
        deepest.my_string = "deepest";

        tst_sub_struct inner;
        inner.my_int = 10;
        inner.my_string = "inner";
        inner.my_vec_sp.push_back(deepest);

        tst_sub_struct outer;
        outer.my_int = 1;
        outer.my_string = "outer";
        outer.my_vec_sp.push_back(inner);

        std::string yaml = prism::yaml::toYamlStringBlock(outer);
        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_string == "outer");
        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 10);
        REQUIRE(result->my_vec_sp[0].my_string == "inner");
        REQUIRE(result->my_vec_sp[0].my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_vec_sp[0].my_int == 999);
        REQUIRE(result->my_vec_sp[0].my_vec_sp[0].my_string == "deepest");
    }

    SECTION("tst_struct vec_sp elements with non-empty sub my_vec_sp block round trip")
    {
        tst_struct obj;
        obj.my_int = 5;

        // sub1 has its own my_vec_sp with one element
        tst_sub_struct sub1;
        sub1.my_int = 100;
        sub1.my_string = "has_children";
        tst_sub_struct grandchild;
        grandchild.my_int = 200;
        grandchild.my_string = "grandchild";
        sub1.my_vec_sp.push_back(grandchild);
        obj.my_vec_sp.push_back(sub1);

        // sub2 has empty my_vec_sp
        tst_sub_struct sub2;
        sub2.my_int = 300;
        sub2.my_string = "no_children";
        obj.my_vec_sp.push_back(sub2);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 100);
        REQUIRE(result->my_vec_sp[0].my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_vec_sp[0].my_int == 200);
        REQUIRE(result->my_vec_sp[0].my_vec_sp[0].my_string == "grandchild");
        REQUIRE(result->my_vec_sp[1].my_int == 300);
        REQUIRE(result->my_vec_sp[1].my_vec_sp.empty());
    }

    SECTION("tst_struct list_sp elements with list_int fields block round trip")
    {
        tst_struct obj;
        obj.my_int = 7;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct elem;
        elem.my_int = 42;
        elem.my_list_int = {1, 2, 3};
        elem.my_list_std_string = {"x", "y"};
        // also clear default values in element's list
        obj.my_list_sp.push_back(std::move(elem));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 7);
        REQUIRE(result->my_list_int.empty());
        REQUIRE(result->my_list_sp.size() == 1);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_int == 42);
        REQUIRE(it->my_list_int.size() == 3);
        REQUIRE(it->my_list_std_string.size() == 2);
    }
}
