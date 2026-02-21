#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format map<string,struct> as struct field", "[yaml][block][map][struct]")
{
    SECTION("tst_struct my_map two entries block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;

        tst_struct v1;
        v1.my_int = 100;
        v1.my_bool = true;
        obj.my_map["alpha"] = std::move(v1);

        tst_struct v2;
        v2.my_int = 200;
        v2.my_bool = false;
        obj.my_map["beta"] = std::move(v2);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_map.size() == 2);
        REQUIRE(result->my_map["alpha"].my_int == 100);
        REQUIRE(result->my_map["alpha"].my_bool == true);
        REQUIRE(result->my_map["beta"].my_int == 200);
        REQUIRE(result->my_map["beta"].my_bool == false);
    }

    SECTION("tst_struct my_map_enum two entries block round trip")
    {
        tst_struct obj;
        obj.my_map_enum["lang1"] = language::english;
        obj.my_map_enum["lang2"] = language::SimplifiedChinese;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map_enum.size() == 2);
        REQUIRE(result->my_map_enum["lang1"] == language::english);
        REQUIRE(result->my_map_enum["lang2"] == language::SimplifiedChinese);
    }
}
