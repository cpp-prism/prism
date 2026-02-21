#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format newer tst_struct fields round trip", "[yaml][flow][unique_ptr][optional][struct]")
{
    SECTION("my_uptr_sub non-null flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 55;
        obj.my_uptr_sub->my_bool = true;
        obj.my_uptr_sub->my_string = "uptr_flow";

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 55);
        REQUIRE(result->my_uptr_sub->my_bool == true);
        REQUIRE(result->my_uptr_sub->my_string == "uptr_flow");
    }

    SECTION("my_uptr_sub null flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // my_uptr_sub is nullptr by default

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_uptr_sub == nullptr);
    }

    SECTION("my_opt_struct with value flow round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        tst_sub_struct sub;
        sub.my_int = 77;
        sub.my_bool = false;
        sub.my_string = "opt_flow";
        obj.my_opt_struct = sub;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 77);
        REQUIRE(result->my_opt_struct->my_bool == false);
        REQUIRE(result->my_opt_struct->my_string == "opt_flow");
    }

    SECTION("my_opt_struct nullopt flow round trip")
    {
        tst_struct obj;
        obj.my_int = 4;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // my_opt_struct is nullopt by default

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 4);
        REQUIRE(!result->my_opt_struct.has_value());
    }

    SECTION("my_uptr_sub and my_opt_struct both set flow round trip")
    {
        tst_struct obj;
        obj.my_int = 5;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 11;
        obj.my_uptr_sub->my_string = "uptr_combined";

        tst_sub_struct sub;
        sub.my_int = 22;
        sub.my_string = "opt_combined";
        obj.my_opt_struct = sub;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 11);
        REQUIRE(result->my_uptr_sub->my_string == "uptr_combined");
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 22);
        REQUIRE(result->my_opt_struct->my_string == "opt_combined");
    }
}
