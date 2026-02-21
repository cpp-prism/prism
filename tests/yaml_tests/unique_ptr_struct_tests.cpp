#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - unique_ptr<struct> field round trip", "[yaml][unique_ptr][struct]")
{
    SECTION("tst_struct my_uptr_sub non-null block round trip")
    {
        tst_struct original;
        original.my_int = 4;
        original.my_uptr_sub = std::make_unique<tst_sub_struct>();
        original.my_uptr_sub->my_int = 77;
        original.my_uptr_sub->my_bool = true;
        original.my_uptr_sub->my_string = "uptr_val";

        std::string yaml = prism::yaml::toYamlStringBlock(original);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 4);
        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 77);
        REQUIRE(result->my_uptr_sub->my_bool == true);
        REQUIRE(result->my_uptr_sub->my_string == "uptr_val");
    }

    SECTION("tst_struct my_uptr_sub null block round trip")
    {
        tst_struct original;
        original.my_int = 2;
        // my_uptr_sub is nullptr by default

        std::string yaml = prism::yaml::toYamlStringBlock(original);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_uptr_sub == nullptr);
    }

    SECTION("standalone unique_ptr<tst_sub_struct> non-null flow round trip")
    {
        std::unique_ptr<tst_sub_struct> original = std::make_unique<tst_sub_struct>();
        original->my_int = 99;
        original->my_string = "standalone_uptr";

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::unique_ptr<tst_sub_struct>>(yaml);

        REQUIRE(*result != nullptr);
        REQUIRE((*result)->my_int == 99);
        REQUIRE((*result)->my_string == "standalone_uptr");
    }

    SECTION("standalone unique_ptr<tst_sub_struct> null flow round trip")
    {
        std::unique_ptr<tst_sub_struct> original;

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::unique_ptr<tst_sub_struct>>(yaml);

        REQUIRE(*result == nullptr);
    }
}
