#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map_enum with multiple entries round trip", "[yaml][block][map_enum][large]")
{
    SECTION("my_map_enum three entries all different values block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map_enum["k1"] = language::english;
        obj.my_map_enum["k2"] = language::SimplifiedChinese;
        obj.my_map_enum["k3"] = language::unknow;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map_enum.size() == 3);
        REQUIRE(result->my_map_enum.at("k1") == language::english);
        REQUIRE(result->my_map_enum.at("k2") == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum.at("k3") == language::unknow);
    }

    SECTION("my_map_enum two entries same value block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map_enum["x"] = language::SimplifiedChinese;
        obj.my_map_enum["y"] = language::SimplifiedChinese;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map_enum.size() == 2);
        REQUIRE(result->my_map_enum.at("x") == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum.at("y") == language::SimplifiedChinese);
    }

    SECTION("my_map_enum empty block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map_enum.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map_enum.empty());
    }
}
