#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format mixed containers round trip", "[yaml][block][containers]")
{
    SECTION("struct with vector<int> containing negative values block round trip")
    {
        // Negative numbers in block arrays should not confuse element boundaries
        tst_struct obj;
        obj.my_deque_int = {-5, 0, 10, -20};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_deque_int.size() == 4);
        REQUIRE(result->my_deque_int[0] == -5);
        REQUIRE(result->my_deque_int[1] == 0);
        REQUIRE(result->my_deque_int[2] == 10);
        REQUIRE(result->my_deque_int[3] == -20);
    }

    SECTION("struct with both non-empty vec_sp and deque_int block round trip")
    {
        tst_struct obj;
        obj.my_int = 99;

        tst_sub_struct sub;
        sub.my_int = 7;
        sub.my_bool = true;
        sub.my_string = "coexist";
        obj.my_vec_sp.push_back(sub);

        obj.my_deque_int = {1, 2, 3};
        obj.my_list_int = {10, 20};
        obj.my_list_std_string = {"a", "b"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 99);
        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 7);
        REQUIRE(result->my_vec_sp[0].my_string == "coexist");
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_deque_int[0] == 1);
        REQUIRE(result->my_list_int.size() == 2);
        REQUIRE(result->my_list_std_string.size() == 2);
    }

    SECTION("struct with set<string> and vec_enum non-empty block round trip")
    {
        tst_struct obj;
        obj.my_set_str = {"foo", "bar", "baz"};
        obj.my_vec_enum = {language::english, language::SimplifiedChinese};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("foo") == 1);
        REQUIRE(result->my_set_str.count("bar") == 1);
        REQUIRE(result->my_set_str.count("baz") == 1);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_vec_enum[1] == language::SimplifiedChinese);
    }

    SECTION("tst_struct all containers empty block round trip (overriding defaults)")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // All other containers are already empty by default

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_list_int.empty());
        REQUIRE(result->my_list_std_string.empty());
        REQUIRE(result->my_vec_sp.empty());
        REQUIRE(result->my_deque_int.empty());
        REQUIRE(result->my_set_str.empty());
        REQUIRE(result->my_vec_enum.empty());
    }
}
