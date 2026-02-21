#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map multiple entries round trip", "[yaml][block][map][multi]")
{
    SECTION("my_map two entries with distinct child scalars block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct c1;
        c1.my_int = 10;
        c1.my_list_int.clear();
        c1.my_list_std_string.clear();
        obj.my_map.emplace("a", std::move(c1));

        tst_struct c2;
        c2.my_int = 20;
        c2.my_list_int.clear();
        c2.my_list_std_string.clear();
        obj.my_map.emplace("b", std::move(c2));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.size() == 2);
        REQUIRE(result->my_map.at("a").my_int == 10);
        REQUIRE(result->my_map.at("b").my_int == 20);
    }

    SECTION("my_map three entries with bool fields block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        for (int i = 0; i < 3; ++i) {
            tst_struct child;
            child.my_int = i;
            child.my_bool = (i % 2 == 0);
            child.my_list_int.clear();
            child.my_list_std_string.clear();
            obj.my_map.emplace("k" + std::to_string(i), std::move(child));
        }

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.size() == 3);
        REQUIRE(result->my_map.at("k0").my_bool == true);
        REQUIRE(result->my_map.at("k1").my_bool == false);
        REQUIRE(result->my_map.at("k2").my_bool == true);
    }

    SECTION("my_map entry with vec_sp child block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 99;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        tst_sub_struct sub;
        sub.my_int = 42;
        child.my_vec_sp.push_back(sub);
        obj.my_map.emplace("main", std::move(child));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map.at("main").my_int == 99);
        REQUIRE(result->my_map.at("main").my_vec_sp.size() == 1);
        REQUIRE(result->my_map.at("main").my_vec_sp[0].my_int == 42);
    }
}
