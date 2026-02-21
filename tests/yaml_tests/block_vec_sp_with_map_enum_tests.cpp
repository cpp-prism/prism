#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_vec_sp elements with my_map_enum round trip", "[yaml][block][vec][struct][enum][map]")
{
    SECTION("my_vec_sp element with my_map_enum block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 5;
        sub.my_string = "sub_block_map_enum";
        obj.my_vec_sp.push_back(sub);

        obj.my_map_enum["en_key"] = language::english;
        obj.my_map_enum["tc_key"] = language::TraditionalChinese;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 5);
        REQUIRE(result->my_vec_sp[0].my_string == "sub_block_map_enum");
        REQUIRE(result->my_map_enum.size() == 2);
        REQUIRE(result->my_map_enum["en_key"] == language::english);
        REQUIRE(result->my_map_enum["tc_key"] == language::TraditionalChinese);
    }

    SECTION("my_vec_sp 2 elements with map_enum 3 entries block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 0; i < 2; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = (i + 1) * 11;
            sub.my_string = "sub_" + std::to_string(i);
            obj.my_vec_sp.push_back(sub);
        }

        obj.my_map_enum["a"] = language::english;
        obj.my_map_enum["b"] = language::SimplifiedChinese;
        obj.my_map_enum["c"] = language::TraditionalChinese;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 11);
        REQUIRE(result->my_vec_sp[1].my_int == 22);
        REQUIRE(result->my_map_enum.size() == 3);
        REQUIRE(result->my_map_enum["a"] == language::english);
        REQUIRE(result->my_map_enum["b"] == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum["c"] == language::TraditionalChinese);
    }
}
