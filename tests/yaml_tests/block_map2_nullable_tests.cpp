#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <map>

TEST_CASE("prismYaml - block format map<string,shared_ptr<struct>> with null values", "[yaml][block][map][shared_ptr][null]")
{
    SECTION("standalone map<string,shared_ptr<tst_sub_struct>> with null block round trip")
    {
        std::map<std::string, std::shared_ptr<tst_sub_struct>> original;
        auto s = std::make_shared<tst_sub_struct>();
        s->my_int = 77;
        s->my_bool = true;
        s->my_string = "live";
        original["alive"] = s;
        original["dead"] = nullptr;

        std::string yaml = prism::yaml::toYamlStringBlock(original, 2);
        auto result = prism::yaml::fromYamlString<std::map<std::string, std::shared_ptr<tst_sub_struct>>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)["alive"] != nullptr);
        REQUIRE((*result)["alive"]->my_int == 77);
        REQUIRE((*result)["alive"]->my_bool == true);
        REQUIRE((*result)["alive"]->my_string == "live");
        REQUIRE((*result)["dead"] == nullptr);
    }

    SECTION("tst_struct my_map2 field block round trip with null entry")
    {
        tst_struct obj;
        obj.my_int = 12;
        // Override the default my_map2 entries
        obj.my_map2.clear();
        obj.my_map2["k_non_null"] = std::make_shared<tst_sub_struct>();
        obj.my_map2["k_non_null"]->my_int = 55;
        obj.my_map2["k_non_null"]->my_string = "val_map2";
        obj.my_map2["k_null"] = nullptr;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 12);
        REQUIRE(result->my_map2.size() == 2);
        REQUIRE(result->my_map2.count("k_non_null") == 1);
        REQUIRE(result->my_map2["k_non_null"] != nullptr);
        REQUIRE(result->my_map2["k_non_null"]->my_int == 55);
        REQUIRE(result->my_map2["k_non_null"]->my_string == "val_map2");
        REQUIRE(result->my_map2.count("k_null") == 1);
        REQUIRE(result->my_map2["k_null"] == nullptr);
    }
}
