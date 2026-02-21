#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_shared_self with container fields populated round trip", "[yaml][block][shared_self][containers][combo]")
{
    SECTION("my_shared_self with deque_int and set_str block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 11;
        obj.my_shared_self->my_list_int.clear();
        obj.my_shared_self->my_list_std_string.clear();
        obj.my_shared_self->my_deque_int = {10, 20};
        obj.my_shared_self->my_set_str = {"red", "blue"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 11);
        REQUIRE(result->my_shared_self->my_deque_int.size() == 2);
        REQUIRE(result->my_shared_self->my_set_str.count("red") == 1);
    }

    SECTION("my_shared_self null alongside root containers block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_shared_self.reset();
        obj.my_deque_int = {5, 10};
        obj.my_set_str = {"alpha", "beta"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_shared_self == nullptr);
        REQUIRE(result->my_deque_int.size() == 2);
        REQUIRE(result->my_set_str.size() == 2);
    }
}
