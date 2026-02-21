#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("prismYaml - block format struct round trip", "[yaml][block][round-trip]")
{
    SECTION("vector<tst_sub_struct> block format serialization and round trip")
    {
        std::vector<tst_sub_struct> original;
        tst_sub_struct s1;
        s1.my_int = 11;
        s1.my_bool = true;
        s1.my_string = "block_one";
        original.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 22;
        s2.my_bool = false;
        s2.my_string = "block_two";
        original.push_back(s2);

        // 生成 block 格式 YAML
        std::string yaml = prism::yaml::toYamlStringBlock(original, 2);
        // block 格式：顶层数组以 "- " 开头，而非 '[' 包裹
        REQUIRE(yaml.find("- ") != std::string::npos);

        // round trip 反序列化
        auto result = prism::yaml::fromYamlString<std::vector<tst_sub_struct>>(yaml);
        REQUIRE(result->size() == 2);
        REQUIRE((*result)[0].my_int == 11);
        REQUIRE((*result)[0].my_bool == true);
        REQUIRE((*result)[0].my_string == "block_one");
        REQUIRE((*result)[1].my_int == 22);
        REQUIRE((*result)[1].my_bool == false);
        REQUIRE((*result)[1].my_string == "block_two");
    }

    SECTION("tst_sub_struct block format serialization and round trip")
    {
        tst_sub_struct original;
        original.my_int = 77;
        original.my_bool = true;
        original.my_float = 1.5f;
        original.my_string = "block_struct";

        std::string yaml = prism::yaml::toYamlStringBlock(original, 2);
        REQUIRE(yaml.find("{") == std::string::npos);
        REQUIRE(yaml.find("my_int:") != std::string::npos);

        auto result = prism::yaml::fromYamlString<tst_sub_struct>(yaml);
        REQUIRE(result->my_int == 77);
        REQUIRE(result->my_bool == true);
        REQUIRE(std::abs(result->my_float - 1.5f) < 0.001f);
        REQUIRE(result->my_string == "block_struct");
    }
}
