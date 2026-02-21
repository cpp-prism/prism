#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - block format my_map2 shared_ptr fields with various sub_struct values round trip", "[yaml][block][map2][shared_ptr]")
{
    SECTION("my_map2 entry with float fields block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 10;
        sub->my_float = 2.5f;
        obj.my_map2["item"] = sub;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map2.size() == 1);
        REQUIRE(result->my_map2.at("item") != nullptr);
        REQUIRE(result->my_map2.at("item")->my_int == 10);
        REQUIRE(result->my_map2.at("item")->my_float == Catch::Approx(2.5f).epsilon(1e-4));
    }

    SECTION("my_map2 two entries one null one non-null block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 99;
        sub->my_longlong = 12345LL;
        obj.my_map2["valid"] = sub;
        obj.my_map2["empty"] = nullptr;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map2.size() == 2);
        REQUIRE(result->my_map2.at("valid") != nullptr);
        REQUIRE(result->my_map2.at("valid")->my_int == 99);
        REQUIRE(result->my_map2.at("valid")->my_longlong == 12345LL);
        REQUIRE(result->my_map2.at("empty") == nullptr);
    }
}
