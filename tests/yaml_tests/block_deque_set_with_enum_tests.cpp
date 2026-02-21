#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_deque_int and my_set_str with my_vec_enum round trip", "[yaml][block][deque][set][vec_enum][combo]")
{
    SECTION("my_deque_int and my_vec_enum together block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {10, 20, 30};
        obj.my_vec_enum = {language::SimplifiedChinese, language::english};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == language::SimplifiedChinese);
        REQUIRE(result->my_vec_enum[1] == language::english);
    }

    SECTION("my_set_str and my_map_enum together block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"red", "blue"};
        obj.my_map_enum["color1"] = language::english;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_set_str.size() == 2);
        REQUIRE(result->my_set_str.count("red") == 1);
        REQUIRE(result->my_map_enum.at("color1") == language::english);
    }
}
