#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_uptr_sub alongside vec_sp round trip", "[yaml][block][uptr][vec_sp][combo]")
{
    SECTION("my_uptr_sub set and vec_sp populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 77;
        obj.my_uptr_sub->my_bool = true;

        tst_sub_struct sub;
        sub.my_int = 55;
        obj.my_vec_sp.push_back(sub);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 77);
        REQUIRE(result->my_uptr_sub->my_bool == true);
        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 55);
    }

    SECTION("my_uptr_sub null and vec_sp empty block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_uptr_sub.reset();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_uptr_sub == nullptr);
        REQUIRE(result->my_vec_sp.empty());
    }

    SECTION("my_uptr_sub with string alongside vec_sp two elements block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 10;
        obj.my_uptr_sub->my_string = "uptr_str";

        tst_sub_struct s1;
        s1.my_int = 1;
        s1.my_string = "v1";
        obj.my_vec_sp.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 2;
        s2.my_string = "v2";
        obj.my_vec_sp.push_back(s2);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_uptr_sub->my_string == "uptr_str");
        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_string == "v1");
        REQUIRE(result->my_vec_sp[1].my_string == "v2");
    }
}
