#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map entries with bool and lang fields round trip", "[yaml][block][map][bool][enum][combo]")
{
    SECTION("my_map entry with bool and lang block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 5;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        child.my_bool = true;
        child.lang = language::english;
        obj.my_map.emplace("node1", std::move(child));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.at("node1").my_bool == true);
        REQUIRE(result->my_map.at("node1").lang == language::english);
    }

    SECTION("root bool and lang with map entry block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();
        obj.my_bool = false;
        obj.lang = language::SimplifiedChinese;

        tst_struct child;
        child.my_int = 77;
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        child.lang = language::unknow;
        obj.my_map.emplace("c", std::move(child));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_bool == false);
        REQUIRE(result->lang == language::SimplifiedChinese);
        REQUIRE(result->my_map.at("c").lang == language::unknow);
    }
}
