#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_list_sp elements with enum and bool fields round trip", "[yaml][block][list_sp][enum][bool][combo]")
{
    SECTION("my_list_sp element with lang field block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct entry;
        entry.my_int = 10;
        entry.my_list_int.clear();
        entry.my_list_std_string.clear();
        entry.lang = language::SimplifiedChinese;
        obj.my_list_sp.push_back(std::move(entry));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_sp.size() == 1);
        REQUIRE(result->my_list_sp.begin()->lang == language::SimplifiedChinese);
    }

    SECTION("my_list_sp two elements with different enum values block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct e1;
        e1.my_int = 1;
        e1.my_list_int.clear();
        e1.my_list_std_string.clear();
        e1.lang = language::english;
        e1.my_bool = true;
        obj.my_list_sp.push_back(std::move(e1));

        tst_struct e2;
        e2.my_int = 2;
        e2.my_list_int.clear();
        e2.my_list_std_string.clear();
        e2.lang = language::unknow;
        e2.my_bool = false;
        obj.my_list_sp.push_back(std::move(e2));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_sp.size() == 2);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->lang == language::english);
        REQUIRE(it->my_bool == true);
        ++it;
        REQUIRE(it->lang == language::unknow);
        REQUIRE(it->my_bool == false);
    }
}
