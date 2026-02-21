#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_deque_int large sizes and mixed values round trip", "[yaml][block][deque][large]")
{
    SECTION("my_deque_int 12 elements block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 1; i <= 12; ++i) {
            obj.my_deque_int.push_back(i * 5);
        }

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_deque_int.size() == 12);
        REQUIRE(result->my_deque_int[0] == 5);
        REQUIRE(result->my_deque_int[11] == 60);
    }

    SECTION("my_deque_int mixed negative, zero, positive block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {-200, -100, 0, 100, 200};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_deque_int.size() == 5);
        REQUIRE(result->my_deque_int[0] == -200);
        REQUIRE(result->my_deque_int[2] == 0);
        REQUIRE(result->my_deque_int[4] == 200);
    }

    SECTION("my_deque_int single element block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {777};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_deque_int.size() == 1);
        REQUIRE(result->my_deque_int[0] == 777);
    }
}
