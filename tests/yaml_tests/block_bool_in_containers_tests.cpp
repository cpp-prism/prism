#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_bool in various struct contexts round trip", "[yaml][block][bool][struct]")
{
    SECTION("my_bool = true in root tst_struct block round trip")
    {
        tst_struct obj;
        obj.my_bool = true;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_bool == true);
    }

    SECTION("my_bool alternating in my_vec_sp elements block round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 0; i < 4; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = i;
            sub.my_bool = (i % 2 == 1);
            obj.my_vec_sp.push_back(sub);
        }

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 4);
        for (int i = 0; i < 4; ++i)
        {
            REQUIRE(result->my_vec_sp[i].my_int == i);
            REQUIRE(result->my_vec_sp[i].my_bool == (i % 2 == 1));
        }
    }

    SECTION("my_bool in my_unordermap values block round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct s1;
        s1.my_int = 1;
        s1.my_bool = true;
        s1.my_string = "true_entry";
        obj.my_unordermap["t"] = s1;

        tst_sub_struct s2;
        s2.my_int = 2;
        s2.my_bool = false;
        s2.my_string = "false_entry";
        obj.my_unordermap["f"] = s2;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_unordermap["t"].my_bool == true);
        REQUIRE(result->my_unordermap["t"].my_string == "true_entry");
        REQUIRE(result->my_unordermap["f"].my_bool == false);
        REQUIRE(result->my_unordermap["f"].my_string == "false_entry");
    }
}
