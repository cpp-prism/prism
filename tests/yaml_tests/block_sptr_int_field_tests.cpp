#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>

TEST_CASE("prismYaml - block format shared_ptr<int> struct field round trip", "[yaml][block][shared_ptr][int]")
{
    SECTION("tst_struct my_sptr_int non-null with custom value block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_sptr_int = std::make_shared<int>(777);
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_sptr_int != nullptr);
        REQUIRE(*result->my_sptr_int == 777);
    }

    SECTION("tst_struct my_sptr_int default value (2) preserved in block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        // my_sptr_int defaults to make_shared<int>(2)
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_sptr_int != nullptr);
        REQUIRE(*result->my_sptr_int == 2);
    }

    SECTION("tst_struct my_sptr_int null block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_sptr_int = nullptr;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_sptr_int == nullptr);
    }
}
