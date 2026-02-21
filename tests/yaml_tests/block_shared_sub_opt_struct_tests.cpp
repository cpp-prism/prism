#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_shared_sub and my_opt_struct combined round trip", "[yaml][block][shared_sub][opt_struct][combo]")
{
    SECTION("both my_shared_sub and my_opt_struct set block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 33;
        obj.my_shared_sub->my_string = "shared";

        obj.my_opt_struct = tst_sub_struct{};
        obj.my_opt_struct->my_int = 44;
        obj.my_opt_struct->my_bool = true;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 33);
        REQUIRE(result->my_shared_sub->my_string == "shared");
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 44);
        REQUIRE(result->my_opt_struct->my_bool == true);
    }

    SECTION("my_shared_sub set, my_opt_struct nullopt block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 55;
        obj.my_opt_struct = std::nullopt;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 55);
        REQUIRE(!result->my_opt_struct.has_value());
    }

    SECTION("my_shared_sub null, my_opt_struct set block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_sub.reset();
        obj.my_opt_struct = tst_sub_struct{};
        obj.my_opt_struct->my_int = 66;
        obj.my_opt_struct->my_longlong = 12345LL;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_shared_sub == nullptr);
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 66);
        REQUIRE(result->my_opt_struct->my_longlong == 12345LL);
    }
}
