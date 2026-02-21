#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_vec_enum and my_map_enum combined round trip", "[yaml][block][vec][map][enum][combo]")
{
    SECTION("my_vec_enum with multiple enum values block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_vec_enum = {language::english, language::SimplifiedChinese, language::unknow};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_enum.size() == 3);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_vec_enum[1] == language::SimplifiedChinese);
        REQUIRE(result->my_vec_enum[2] == language::unknow);
    }

    SECTION("my_map_enum with multiple enum values block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map_enum["en"] = language::english;
        obj.my_map_enum["zh"] = language::SimplifiedChinese;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map_enum.size() == 2);
        REQUIRE(result->my_map_enum.at("en") == language::english);
        REQUIRE(result->my_map_enum.at("zh") == language::SimplifiedChinese);
    }

    SECTION("my_vec_enum and my_map_enum together block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_vec_enum = {language::unknow, language::english};
        obj.my_map_enum["first"] = language::SimplifiedChinese;
        obj.my_map_enum["second"] = language::english;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == language::unknow);
        REQUIRE(result->my_vec_enum[1] == language::english);
        REQUIRE(result->my_map_enum.size() == 2);
        REQUIRE(result->my_map_enum.at("first") == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum.at("second") == language::english);
    }
}
