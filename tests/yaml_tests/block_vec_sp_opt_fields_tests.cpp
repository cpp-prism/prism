#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_vec_sp elements with optional fields round trip", "[yaml][block][vec][optional][fields]")
{
    SECTION("my_vec_sp element with opt_str populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 10;
        sub.my_opt_str = "sub_optional";
        obj.my_vec_sp.push_back(sub);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 10);
        REQUIRE(result->my_vec_sp[0].my_opt_str.has_value());
        REQUIRE(result->my_vec_sp[0].my_opt_str.value() == "sub_optional");
    }

    SECTION("my_vec_sp two elements with different opt_int values block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct s1;
        s1.my_int = 1;
        s1.my_opt_int = 100;
        obj.my_vec_sp.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 2;
        s2.my_opt_int = std::nullopt;
        obj.my_vec_sp.push_back(s2);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_opt_int.has_value());
        REQUIRE(result->my_vec_sp[0].my_opt_int.value() == 100);
        REQUIRE(!result->my_vec_sp[1].my_opt_int.has_value());
    }
}
