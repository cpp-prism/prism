#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_sptr_int alongside vec_sp round trip", "[yaml][block][sptr_int][vec_sp][combo]")
{
    SECTION("my_sptr_int set and vec_sp two elements block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_sptr_int = std::make_shared<int>(42);

        tst_sub_struct s1;
        s1.my_int = 10;
        obj.my_vec_sp.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 20;
        obj.my_vec_sp.push_back(s2);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_sptr_int != nullptr);
        REQUIRE(*result->my_sptr_int == 42);
        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 10);
        REQUIRE(result->my_vec_sp[1].my_int == 20);
    }

    SECTION("my_sptr_int null and vec_sp empty block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_sptr_int.reset();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_sptr_int == nullptr);
        REQUIRE(result->my_vec_sp.empty());
    }

    SECTION("my_sptr_int positive and vec_sp one element with bool block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_sptr_int = std::make_shared<int>(77);

        tst_sub_struct sub;
        sub.my_int = 5;
        sub.my_bool = true;
        obj.my_vec_sp.push_back(sub);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(*result->my_sptr_int == 77);
        REQUIRE(result->my_vec_sp[0].my_bool == true);
    }
}
