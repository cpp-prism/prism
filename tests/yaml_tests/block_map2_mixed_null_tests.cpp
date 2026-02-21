#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map2 mixed null and non-null entries round trip", "[yaml][block][map][shared_ptr][null]")
{
    SECTION("my_map2 single non-null entry block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 42;
        sub->my_string = "block_map2_non_null";
        obj.my_map2["key_a"] = sub;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map2.count("key_a") == 1);
        REQUIRE(result->my_map2["key_a"] != nullptr);
        REQUIRE(result->my_map2["key_a"]->my_int == 42);
        REQUIRE(result->my_map2["key_a"]->my_string == "block_map2_non_null");
    }

    SECTION("my_map2 null entry block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        obj.my_map2["null_key"] = nullptr;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map2.count("null_key") == 1);
        REQUIRE(result->my_map2["null_key"] == nullptr);
    }

    SECTION("my_map2 mixed null and non-null entries block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 88;
        sub->my_bool = true;
        obj.my_map2["active"] = sub;
        obj.my_map2["inactive"] = nullptr;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map2.size() == 2);
        REQUIRE(result->my_map2["active"] != nullptr);
        REQUIRE(result->my_map2["active"]->my_int == 88);
        REQUIRE(result->my_map2["active"]->my_bool == true);
        REQUIRE(result->my_map2["inactive"] == nullptr);
    }
}
