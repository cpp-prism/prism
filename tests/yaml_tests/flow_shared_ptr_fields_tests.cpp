#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format shared_ptr fields round trip", "[yaml][flow][shared_ptr][struct]")
{
    SECTION("my_shared_sub non-null flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 42;
        obj.my_shared_sub->my_bool = true;
        obj.my_shared_sub->my_string = "flow_shared_sub";

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 42);
        REQUIRE(result->my_shared_sub->my_bool == true);
        REQUIRE(result->my_shared_sub->my_string == "flow_shared_sub");
    }

    SECTION("my_shared_sub null flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // my_shared_sub is nullptr by default

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_shared_sub == nullptr);
    }

    SECTION("my_shared_self 1-level chain flow round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 33;
        obj.my_shared_self->my_list_int.clear();
        obj.my_shared_self->my_list_std_string.clear();

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 33);
    }

    SECTION("my_shared_self null flow round trip")
    {
        tst_struct obj;
        obj.my_int = 4;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // my_shared_self nullptr by default

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 4);
        REQUIRE(result->my_shared_self == nullptr);
    }

    SECTION("my_shared_sub with my_vec_sp populated flow round trip")
    {
        tst_struct obj;
        obj.my_int = 5;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 50;
        obj.my_shared_sub->my_string = "parent_flow";

        tst_sub_struct child;
        child.my_int = 51;
        child.my_string = "child_flow";
        obj.my_shared_sub->my_vec_sp.push_back(child);

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 50);
        REQUIRE(result->my_shared_sub->my_string == "parent_flow");
        REQUIRE(result->my_shared_sub->my_vec_sp.size() == 1);
        REQUIRE(result->my_shared_sub->my_vec_sp[0].my_int == 51);
        REQUIRE(result->my_shared_sub->my_vec_sp[0].my_string == "child_flow");
    }
}
