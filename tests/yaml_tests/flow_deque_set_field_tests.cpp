#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_deque_int and my_set_str as struct fields round trip", "[yaml][flow][deque][set]")
{
    SECTION("tst_struct my_deque_int with values flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {5, 10, 15, 20};

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_deque_int.size() == 4);
        REQUIRE(result->my_deque_int[0] == 5);
        REQUIRE(result->my_deque_int[1] == 10);
        REQUIRE(result->my_deque_int[2] == 15);
        REQUIRE(result->my_deque_int[3] == 20);
    }

    SECTION("tst_struct my_set_str with values flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"apple", "banana", "cherry"};

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("apple") == 1);
        REQUIRE(result->my_set_str.count("banana") == 1);
        REQUIRE(result->my_set_str.count("cherry") == 1);
    }

    SECTION("tst_struct my_deque_int empty flow round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // my_deque_int empty by default

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_deque_int.empty());
    }

    SECTION("tst_struct my_set_str single element flow round trip")
    {
        tst_struct obj;
        obj.my_int = 4;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"solo"};

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 4);
        REQUIRE(result->my_set_str.size() == 1);
        REQUIRE(result->my_set_str.count("solo") == 1);
    }
}
