#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_shared_self with enum and container fields", "[yaml][block][shared_ptr][recursive][enum]")
{
    SECTION("my_shared_self child has my_vec_enum populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.lang = language::english;

        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 10;
        obj.my_shared_self->my_list_int.clear();
        obj.my_shared_self->my_list_std_string.clear();
        obj.my_shared_self->my_vec_enum = {language::SimplifiedChinese, language::TraditionalChinese};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->lang == language::english);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 10);
        REQUIRE(result->my_shared_self->my_vec_enum.size() == 2);
        REQUIRE(result->my_shared_self->my_vec_enum[0] == language::SimplifiedChinese);
        REQUIRE(result->my_shared_self->my_vec_enum[1] == language::TraditionalChinese);
    }

    SECTION("my_shared_self child has my_deque_int and my_set_str block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 20;
        obj.my_shared_self->my_list_int.clear();
        obj.my_shared_self->my_list_std_string.clear();
        obj.my_shared_self->my_deque_int = {10, 20, 30};
        obj.my_shared_self->my_set_str = {"x", "y", "z"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 20);
        REQUIRE(result->my_shared_self->my_deque_int.size() == 3);
        REQUIRE(result->my_shared_self->my_deque_int[0] == 10);
        REQUIRE(result->my_shared_self->my_deque_int[2] == 30);
        REQUIRE(result->my_shared_self->my_set_str.count("x") == 1);
        REQUIRE(result->my_shared_self->my_set_str.count("z") == 1);
    }
}
