#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map with special key values round trip", "[yaml][block][map][keys]")
{
    SECTION("my_map with single-char keys block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct v1;
        v1.my_int = 10;
        v1.my_list_int.clear();
        v1.my_list_std_string.clear();
        obj.my_map["a"] = std::move(v1);

        tst_struct v2;
        v2.my_int = 20;
        v2.my_list_int.clear();
        v2.my_list_std_string.clear();
        obj.my_map["b"] = std::move(v2);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_map.size() == 2);
        REQUIRE(result->my_map["a"].my_int == 10);
        REQUIRE(result->my_map["b"].my_int == 20);
    }

    SECTION("my_unordermap with numeric-string keys block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct s1;
        s1.my_int = 100;
        s1.my_string = "val_1";
        obj.my_unordermap["1"] = s1;

        tst_sub_struct s2;
        s2.my_int = 200;
        s2.my_string = "val_2";
        obj.my_unordermap["2"] = s2;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_unordermap.size() == 2);
        REQUIRE(result->my_unordermap["1"].my_int == 100);
        REQUIRE(result->my_unordermap["1"].my_string == "val_1");
        REQUIRE(result->my_unordermap["2"].my_int == 200);
    }

    SECTION("my_map2 with single non-null entry and map2 already cleared block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 77;
        sub->my_bool = true;
        sub->my_string = "block_map2_key";
        obj.my_map2["block_key"] = sub;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_map2.count("block_key") == 1);
        REQUIRE(result->my_map2["block_key"] != nullptr);
        REQUIRE(result->my_map2["block_key"]->my_int == 77);
        REQUIRE(result->my_map2["block_key"]->my_string == "block_map2_key");
    }
}
