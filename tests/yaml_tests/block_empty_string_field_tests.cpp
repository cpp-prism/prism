#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format empty string in struct field", "[yaml][block][string][empty]")
{
    SECTION("tst_sub_struct with my_string=empty block round trip")
    {
        tst_sub_struct obj;
        obj.my_int = 7;
        obj.my_string = "";  // explicitly empty

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        // Empty string must be quoted as "" to distinguish from missing value
        REQUIRE(yaml.find("my_string: \"\"") != std::string::npos);

        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_int == 7);
        REQUIRE(result->my_string == "");
    }

    SECTION("vector of structs with empty string element in block format round trip")
    {
        std::vector<tst_sub_struct> original;

        tst_sub_struct s1;
        s1.my_int = 1;
        s1.my_string = "nonempty";
        original.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 2;
        s2.my_string = "";  // empty in middle element
        original.push_back(s2);

        tst_sub_struct s3;
        s3.my_int = 3;
        s3.my_string = "also_nonempty";
        original.push_back(s3);

        std::string yaml = prism::yaml::toYamlStringBlock(original, 2);
        auto result = prism::yaml::fromYamlString<std::vector<tst_sub_struct>>(yaml);

        REQUIRE(result->size() == 3);
        REQUIRE((*result)[0].my_int == 1);
        REQUIRE((*result)[0].my_string == "nonempty");
        REQUIRE((*result)[1].my_int == 2);
        REQUIRE((*result)[1].my_string == "");
        REQUIRE((*result)[2].my_int == 3);
        REQUIRE((*result)[2].my_string == "also_nonempty");
    }

    SECTION("tst_struct my_vec_sp element with empty string in block round trip")
    {
        tst_struct obj;
        obj.my_int = 5;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 99;
        sub.my_string = "";  // empty string in sub-struct element
        obj.my_vec_sp.push_back(sub);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 99);
        REQUIRE(result->my_vec_sp[0].my_string == "");
    }
}
