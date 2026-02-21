#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_list_sp multiple entries with different field combos round trip", "[yaml][block][list][struct][combo]")
{
    SECTION("my_list_sp 3 entries each with different field states block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct e1;
        e1.my_int = 10;
        e1.my_list_int.clear();
        e1.my_list_std_string.clear();
        e1.my_string = "block_entry_one";
        e1.lang = language::english;
        obj.my_list_sp.push_back(std::move(e1));

        tst_struct e2;
        e2.my_int = 20;
        e2.my_list_int.clear();
        e2.my_list_std_string.clear();
        e2.my_bool = true;
        e2.my_vec_enum = {language::TraditionalChinese};
        obj.my_list_sp.push_back(std::move(e2));

        tst_struct e3;
        e3.my_int = 30;
        e3.my_list_int.clear();
        e3.my_list_std_string.clear();
        e3.my_deque_int = {5, 10};
        e3.my_set_str = {"p", "q"};
        obj.my_list_sp.push_back(std::move(e3));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_sp.size() == 3);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_int == 10);
        REQUIRE(it->lang == language::english);
        ++it;
        REQUIRE(it->my_int == 20);
        REQUIRE(it->my_bool == true);
        REQUIRE(it->my_vec_enum.size() == 1);
        REQUIRE(it->my_vec_enum[0] == language::TraditionalChinese);
        ++it;
        REQUIRE(it->my_int == 30);
        REQUIRE(it->my_deque_int.size() == 2);
        REQUIRE(it->my_set_str.count("p") == 1);
    }

    SECTION("my_list_sp entries each with shared_sub block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 1; i <= 2; ++i)
        {
            tst_struct e;
            e.my_int = i * 100;
            e.my_list_int.clear();
            e.my_list_std_string.clear();
            e.my_shared_sub = std::make_shared<tst_sub_struct>();
            e.my_shared_sub->my_int = i * 1000;
            e.my_shared_sub->my_string = "block_sub_" + std::to_string(i);
            obj.my_list_sp.push_back(std::move(e));
        }

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_sp.size() == 2);
        auto it = result->my_list_sp.begin();
        for (int i = 1; i <= 2; ++i, ++it)
        {
            REQUIRE(it->my_int == i * 100);
            REQUIRE(it->my_shared_sub != nullptr);
            REQUIRE(it->my_shared_sub->my_int == i * 1000);
        }
    }
}
