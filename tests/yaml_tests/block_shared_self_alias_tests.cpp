#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_shared_self recursive struct round trip", "[yaml][block][shared_self][recursive]")
{
    SECTION("my_shared_self null block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_shared_self.reset();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_shared_self == nullptr);
    }

    SECTION("my_shared_self single level block round trip")
    {
        tst_struct obj;
        obj.my_int = 10;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 20;
        obj.my_shared_self->my_list_int.clear();
        obj.my_shared_self->my_list_std_string.clear();
        obj.my_shared_self->my_shared_self.reset();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 10);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 20);
        REQUIRE(result->my_shared_self->my_shared_self == nullptr);
    }
}
