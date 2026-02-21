#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format tst_struct lang field combined with other fields round trip", "[yaml][block][lang][enum][combo]")
{
    SECTION("lang field english with list block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int = {5, 10};
        obj.my_list_std_string.clear();
        obj.lang = language::english;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->lang == language::english);
        REQUIRE(result->my_list_int.size() == 2);
    }

    SECTION("lang field SimplifiedChinese with vec_enum block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.lang = language::SimplifiedChinese;
        obj.my_vec_enum = {language::unknow, language::english};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->lang == language::SimplifiedChinese);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == language::unknow);
        REQUIRE(result->my_vec_enum[1] == language::english);
    }
}
