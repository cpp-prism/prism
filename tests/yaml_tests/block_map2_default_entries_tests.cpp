#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_map2 default entries round trip", "[yaml][block][map][shared_ptr][default]")
{
    SECTION("tst_struct my_map2 with default entries (key1=non-null, key2=null) block round trip")
    {
        // my_map2 defaults to {{"key1", make_shared<tst_sub_struct>()}, {"key2", {}}}
        // key2 is a default-constructed shared_ptr, i.e. nullptr
        tst_struct obj;
        obj.my_int = 9;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // Do NOT call obj.my_map2.clear() — use the defaults

        REQUIRE(obj.my_map2.count("key1") == 1);
        REQUIRE(obj.my_map2["key1"] != nullptr);
        REQUIRE(obj.my_map2.count("key2") == 1);
        REQUIRE(obj.my_map2["key2"] == nullptr);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 9);
        REQUIRE(result->my_map2.size() == 2);
        REQUIRE(result->my_map2.count("key1") == 1);
        REQUIRE(result->my_map2["key1"] != nullptr);
        REQUIRE(result->my_map2.count("key2") == 1);
        REQUIRE(result->my_map2["key2"] == nullptr);
    }

    SECTION("tst_struct my_map2 default key1 entry has non-null struct with default int=0")
    {
        tst_struct obj;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // Default key1 entry is a default-constructed tst_sub_struct (my_int=0, etc.)

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map2["key1"] != nullptr);
        REQUIRE(result->my_map2["key1"]->my_int == 0);
        REQUIRE(result->my_map2["key1"]->my_bool == false);
        REQUIRE(result->my_map2["key1"]->my_string == "");
    }
}
