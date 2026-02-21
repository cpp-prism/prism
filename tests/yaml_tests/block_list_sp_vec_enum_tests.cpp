#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_list_sp elements with vec_enum and deque_int round trip", "[yaml][block][list_sp][vec_enum][deque][combo]")
{
    SECTION("list_sp element with vec_enum populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct entry;
        entry.my_int = 10;
        entry.my_vec_enum = {language::english, language::SimplifiedChinese};
        entry.my_list_int.clear();
        entry.my_list_std_string.clear();
        obj.my_list_sp.push_back(std::move(entry));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_sp.size() == 1);
        REQUIRE(result->my_list_sp.front().my_vec_enum.size() == 2);
        REQUIRE(result->my_list_sp.front().my_vec_enum[0] == language::english);
    }

    SECTION("list_sp element with deque_int populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct entry;
        entry.my_int = 20;
        entry.my_deque_int = {5, 10, 15};
        entry.my_list_int.clear();
        entry.my_list_std_string.clear();
        obj.my_list_sp.push_back(std::move(entry));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_sp.front().my_deque_int.size() == 3);
        REQUIRE(result->my_list_sp.front().my_deque_int[0] == 5);
    }
}
