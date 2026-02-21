#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format tst_struct lang field with map containers round trip", "[yaml][block][lang][map][combo]")
{
    SECTION("lang english with non-empty map block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.lang = language::english;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 5;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        obj.my_map.emplace("en_child", std::move(child));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->lang == language::english);
        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map.at("en_child").my_int == 5);
    }

    SECTION("lang SimplifiedChinese with map_enum block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.lang = language::SimplifiedChinese;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map_enum["k1"] = language::english;
        obj.my_map_enum["k2"] = language::unknow;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->lang == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum.at("k1") == language::english);
    }

    SECTION("lang unknow with empty map block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.lang = language::unknow;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->lang == language::unknow);
        REQUIRE(result->my_map.empty());
    }
}
