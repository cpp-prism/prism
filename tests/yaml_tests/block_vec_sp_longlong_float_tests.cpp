#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - block format my_vec_sp elements with longlong and float/double fields round trip", "[yaml][block][vec][longlong][float][double]")
{
    SECTION("my_vec_sp element with longlong and small double block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 10;
        sub.my_longlong = 1234567890LL;
        sub.my_float = 0.25f;
        sub.my_double = 0.007;
        obj.my_vec_sp.push_back(sub);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_longlong == 1234567890LL);
        REQUIRE(result->my_vec_sp[0].my_float == Catch::Approx(0.25f).epsilon(1e-5));
        REQUIRE(result->my_vec_sp[0].my_double == Catch::Approx(0.007).epsilon(1e-9));
    }

    SECTION("my_vec_sp 2 elements with pi and e values block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub1;
        sub1.my_int = 1;
        sub1.my_double = 3.14159265358979;
        obj.my_vec_sp.push_back(sub1);

        tst_sub_struct sub2;
        sub2.my_int = 2;
        sub2.my_double = 2.71828182845905;
        obj.my_vec_sp.push_back(sub2);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp[0].my_double == Catch::Approx(3.14159265358979).epsilon(1e-9));
        REQUIRE(result->my_vec_sp[1].my_double == Catch::Approx(2.71828182845905).epsilon(1e-9));
    }
}
