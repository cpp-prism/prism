#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_bool in various struct contexts round trip", "[yaml][flow][bool][struct]")
{
    SECTION("my_bool = true in root tst_struct flow round trip")
    {
        tst_struct obj;
        obj.my_bool = true;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_bool == true);
    }

    SECTION("my_bool = false in embedded my_struct flow round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_bool = false;
        obj.my_struct.my_int = 7;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_struct.my_bool == false);
        REQUIRE(result->my_struct.my_int == 7);
    }

    SECTION("my_bool alternating in my_vec_sp elements flow round trip")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 0; i < 4; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = i;
            sub.my_bool = (i % 2 == 0);
            obj.my_vec_sp.push_back(sub);
        }

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 4);
        for (int i = 0; i < 4; ++i)
        {
            REQUIRE(result->my_vec_sp[i].my_int == i);
            REQUIRE(result->my_vec_sp[i].my_bool == (i % 2 == 0));
        }
    }
}
