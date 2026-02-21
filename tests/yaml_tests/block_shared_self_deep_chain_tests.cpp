#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_shared_self deep chain with different fields round trip", "[yaml][block][shared_ptr][self][chain]")
{
    SECTION("my_shared_self with vec_enum and nested child block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_vec_enum = {language::english};

        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 100;
        obj.my_shared_self->my_list_int.clear();
        obj.my_shared_self->my_list_std_string.clear();
        obj.my_shared_self->lang = language::TraditionalChinese;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_vec_enum.size() == 1);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 100);
        REQUIRE(result->my_shared_self->lang == language::TraditionalChinese);
    }

    SECTION("my_shared_self child with deque and set block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 200;
        obj.my_shared_self->my_list_int.clear();
        obj.my_shared_self->my_list_std_string.clear();
        obj.my_shared_self->my_deque_int = {7, 8, 9};
        obj.my_shared_self->my_set_str = {"s1", "s2"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_shared_self->my_int == 200);
        REQUIRE(result->my_shared_self->my_deque_int.size() == 3);
        REQUIRE(result->my_shared_self->my_deque_int[1] == 8);
        REQUIRE(result->my_shared_self->my_set_str.size() == 2);
        REQUIRE(result->my_shared_self->my_set_str.count("s1") == 1);
    }
}
