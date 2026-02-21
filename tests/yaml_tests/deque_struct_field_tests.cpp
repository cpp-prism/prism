#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - deque<int> struct field round trip", "[yaml][deque][struct]")
{
    SECTION("tst_struct my_deque_int with values block round trip")
    {
        tst_struct original;
        original.my_deque_int = {10, 20, 30};

        std::string yaml = prism::yaml::toYamlStringBlock(original);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_deque_int[0] == 10);
        REQUIRE(result->my_deque_int[1] == 20);
        REQUIRE(result->my_deque_int[2] == 30);
    }

    SECTION("tst_struct my_deque_int empty block round trip")
    {
        tst_struct original;
        // my_deque_int is empty by default

        std::string yaml = prism::yaml::toYamlStringBlock(original);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_deque_int.empty());
    }

    SECTION("standalone deque<int> block round trip")
    {
        std::deque<int> original = {5, 15, 25, 35};

        std::string yaml = prism::yaml::toYamlStringBlock(original);
        auto result = prism::yaml::fromYamlString<std::deque<int>>(yaml);

        REQUIRE(result->size() == 4);
        REQUIRE((*result)[0] == 5);
        REQUIRE((*result)[1] == 15);
        REQUIRE((*result)[2] == 25);
        REQUIRE((*result)[3] == 35);
    }

    SECTION("standalone deque<int> flow round trip")
    {
        std::deque<int> original = {1, 2, 3};

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::deque<int>>(yaml);

        REQUIRE(result->size() == 3);
        REQUIRE((*result)[0] == 1);
        REQUIRE((*result)[1] == 2);
        REQUIRE((*result)[2] == 3);
    }
}
