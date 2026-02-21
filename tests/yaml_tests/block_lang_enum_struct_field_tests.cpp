#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format lang enum field in tst_struct round trip", "[yaml][block][enum][struct]")
{
    SECTION("tst_struct lang=english block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.lang = language::english;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->lang == language::english);
    }

    SECTION("tst_struct lang=TraditionalChinese block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.lang = language::TraditionalChinese;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->lang == language::TraditionalChinese);
    }

    SECTION("tst_struct lang=unknow (default=0) block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.lang = language::unknow;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->lang == language::unknow);
    }
}
