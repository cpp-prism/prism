#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_unordermap with my_vec_enum together round trip", "[yaml][block][unordermap][vec_enum][combo]")
{
    SECTION("my_unordermap and my_vec_enum both populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_unordermap.clear();
        obj.my_vec_enum = {language::english, language::unknow};

        tst_sub_struct sub;
        sub.my_int = 7;
        obj.my_unordermap["bkey"] = sub;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_vec_enum[1] == language::unknow);
        REQUIRE(result->my_unordermap.size() == 1);
        REQUIRE(result->my_unordermap.at("bkey").my_int == 7);
    }

    SECTION("my_unordermap with map_enum block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_unordermap.clear();
        obj.my_map_enum["lang_a"] = language::SimplifiedChinese;

        tst_sub_struct sub;
        sub.my_int = 15;
        obj.my_unordermap["uk"] = sub;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map_enum.at("lang_a") == language::SimplifiedChinese);
        REQUIRE(result->my_unordermap.at("uk").my_int == 15);
    }
}
