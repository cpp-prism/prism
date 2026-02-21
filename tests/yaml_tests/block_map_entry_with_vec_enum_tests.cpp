#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map entries with my_vec_enum populated round trip", "[yaml][block][map][struct][enum]")
{
    SECTION("my_map entry with my_vec_enum and lang block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct map_val;
        map_val.my_int = 10;
        map_val.my_list_int.clear();
        map_val.my_list_std_string.clear();
        map_val.lang = language::TraditionalChinese;
        map_val.my_vec_enum = {language::english, language::SimplifiedChinese};
        obj.my_map["block_enum_key"] = std::move(map_val);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map["block_enum_key"].my_int == 10);
        REQUIRE(result->my_map["block_enum_key"].lang == language::TraditionalChinese);
        REQUIRE(result->my_map["block_enum_key"].my_vec_enum.size() == 2);
        REQUIRE(result->my_map["block_enum_key"].my_vec_enum[0] == language::english);
        REQUIRE(result->my_map["block_enum_key"].my_vec_enum[1] == language::SimplifiedChinese);
    }
}
