#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_vec_sp elements each with their own my_vec_sp round trip", "[yaml][block][vec_sp][nested][combo]")
{
    SECTION("my_vec_sp element with its own vec_sp populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 10;
        tst_sub_struct inner;
        inner.my_int = 100;
        inner.my_longlong = 1000LL;
        sub.my_vec_sp.push_back(inner);
        obj.my_vec_sp.push_back(sub);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 10);
        REQUIRE(result->my_vec_sp[0].my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_vec_sp[0].my_int == 100);
        REQUIRE(result->my_vec_sp[0].my_vec_sp[0].my_longlong == 1000LL);
    }

    SECTION("my_vec_sp two elements each with different inner vec_sp block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct s1;
        s1.my_int = 1;
        tst_sub_struct i1;
        i1.my_int = 11;
        s1.my_vec_sp.push_back(i1);
        obj.my_vec_sp.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 2;
        tst_sub_struct i2a;
        i2a.my_int = 21;
        tst_sub_struct i2b;
        i2b.my_int = 22;
        s2.my_vec_sp.push_back(i2a);
        s2.my_vec_sp.push_back(i2b);
        obj.my_vec_sp.push_back(s2);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_vec_sp[0].my_int == 11);
        REQUIRE(result->my_vec_sp[1].my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[1].my_vec_sp[0].my_int == 21);
        REQUIRE(result->my_vec_sp[1].my_vec_sp[1].my_int == 22);
    }
}
