#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>

TEST_CASE("prismYaml - block format shared_ptr<struct> field round trip", "[yaml][block][shared_ptr][struct]")
{
    SECTION("tst_struct my_shared_sub non-null block round trip")
    {
        tst_struct obj;
        obj.my_int = 11;
        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 55;
        obj.my_shared_sub->my_bool = true;
        obj.my_shared_sub->my_string = "shared_block";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 11);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 55);
        REQUIRE(result->my_shared_sub->my_bool == true);
        REQUIRE(result->my_shared_sub->my_string == "shared_block");
    }

    SECTION("tst_struct my_shared_sub null block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        // my_shared_sub defaults to null

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_shared_sub == nullptr);
    }

    SECTION("tst_struct my_shared_self non-null block round trip")
    {
        tst_struct obj;
        obj.my_int = 7;
        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 88;
        obj.my_shared_self->my_bool = true;
        obj.my_shared_self->my_string = "inner_block";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_shared_self->my_list_int.clear();
        obj.my_shared_self->my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 7);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 88);
        REQUIRE(result->my_shared_self->my_bool == true);
        REQUIRE(result->my_shared_self->my_string == "inner_block");
    }
}
