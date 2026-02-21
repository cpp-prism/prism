#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_list_sp elements with enum fields round trip", "[yaml][block][list][struct][enum]")
{
    SECTION("my_list_sp element has my_vec_enum populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct elem;
        elem.my_int = 10;
        elem.my_list_int.clear();
        elem.my_list_std_string.clear();
        elem.lang = language::SimplifiedChinese;
        elem.my_vec_enum = {language::english, language::TraditionalChinese};
        obj.my_list_sp.push_back(std::move(elem));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_list_sp.size() == 1);
        REQUIRE(result->my_list_sp.front().my_int == 10);
        REQUIRE(result->my_list_sp.front().lang == language::SimplifiedChinese);
        REQUIRE(result->my_list_sp.front().my_vec_enum.size() == 2);
        REQUIRE(result->my_list_sp.front().my_vec_enum[0] == language::english);
        REQUIRE(result->my_list_sp.front().my_vec_enum[1] == language::TraditionalChinese);
    }

    SECTION("my_list_sp 2 elements each with deque and set block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct elem1;
        elem1.my_int = 10;
        elem1.my_list_int.clear();
        elem1.my_list_std_string.clear();
        elem1.my_deque_int = {1, 2, 3};
        obj.my_list_sp.push_back(std::move(elem1));

        tst_struct elem2;
        elem2.my_int = 20;
        elem2.my_list_int.clear();
        elem2.my_list_std_string.clear();
        elem2.my_set_str = {"x", "y"};
        obj.my_list_sp.push_back(std::move(elem2));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_list_sp.size() == 2);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_int == 10);
        REQUIRE(it->my_deque_int.size() == 3);
        ++it;
        REQUIRE(it->my_int == 20);
        REQUIRE(it->my_set_str.size() == 2);
    }
}
