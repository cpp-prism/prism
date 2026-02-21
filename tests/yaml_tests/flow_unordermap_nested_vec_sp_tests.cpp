#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_unordermap entries with nested my_vec_sp round trip", "[yaml][flow][unordered_map][struct][vector]")
{
    SECTION("my_unordermap entry with 2 my_vec_sp elements flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct s;
        s.my_int = 10;
        s.my_string = "map_parent";

        tst_sub_struct child1;
        child1.my_int = 11;
        child1.my_string = "map_child_one";
        s.my_vec_sp.push_back(child1);

        tst_sub_struct child2;
        child2.my_int = 22;
        child2.my_string = "map_child_two";
        s.my_vec_sp.push_back(child2);

        obj.my_unordermap["parent"] = s;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_unordermap.size() == 1);
        REQUIRE(result->my_unordermap["parent"].my_int == 10);
        REQUIRE(result->my_unordermap["parent"].my_string == "map_parent");
        REQUIRE(result->my_unordermap["parent"].my_vec_sp.size() == 2);
        REQUIRE(result->my_unordermap["parent"].my_vec_sp[0].my_int == 11);
        REQUIRE(result->my_unordermap["parent"].my_vec_sp[0].my_string == "map_child_one");
        REQUIRE(result->my_unordermap["parent"].my_vec_sp[1].my_int == 22);
        REQUIRE(result->my_unordermap["parent"].my_vec_sp[1].my_string == "map_child_two");
    }

    SECTION("my_unordermap two entries each with 1 my_vec_sp element flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 1; i <= 2; ++i)
        {
            tst_sub_struct s;
            s.my_int = i * 10;
            s.my_string = "entry_" + std::to_string(i);

            tst_sub_struct child;
            child.my_int = i * 100;
            child.my_string = "child_" + std::to_string(i);
            s.my_vec_sp.push_back(child);

            obj.my_unordermap["key" + std::to_string(i)] = s;
        }

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_unordermap.size() == 2);
        for (int i = 1; i <= 2; ++i)
        {
            std::string k = "key" + std::to_string(i);
            REQUIRE(result->my_unordermap[k].my_int == i * 10);
            REQUIRE(result->my_unordermap[k].my_vec_sp.size() == 1);
            REQUIRE(result->my_unordermap[k].my_vec_sp[0].my_int == i * 100);
        }
    }
}
