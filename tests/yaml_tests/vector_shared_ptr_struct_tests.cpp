#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <vector>

TEST_CASE("prismYaml - vector<shared_ptr<struct>> round trip", "[yaml][vector][smart-pointer][struct]")
{
    SECTION("vector<shared_ptr<tst_sub_struct>> all non-null round trip")
    {
        std::vector<std::shared_ptr<tst_sub_struct>> original;
        auto s1 = std::make_shared<tst_sub_struct>();
        s1->my_int = 11;
        s1->my_bool = true;
        s1->my_float = 1.5f;
        s1->my_string = "item_one";
        original.push_back(s1);
        auto s2 = std::make_shared<tst_sub_struct>();
        s2->my_int = 22;
        s2->my_bool = false;
        s2->my_float = 2.5f;
        s2->my_string = "item_two";
        original.push_back(s2);

        // 使用 flow 格式保证 round trip 正确性
        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::vector<std::shared_ptr<tst_sub_struct>>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)[0] != nullptr);
        REQUIRE((*result)[0]->my_int == 11);
        REQUIRE((*result)[0]->my_bool == true);
        REQUIRE((*result)[0]->my_string == "item_one");
        REQUIRE((*result)[1] != nullptr);
        REQUIRE((*result)[1]->my_int == 22);
        REQUIRE((*result)[1]->my_bool == false);
        REQUIRE((*result)[1]->my_string == "item_two");
    }

    SECTION("vector<shared_ptr<tst_sub_struct>> with null elements round trip")
    {
        std::vector<std::shared_ptr<tst_sub_struct>> original;
        auto s = std::make_shared<tst_sub_struct>();
        s->my_int = 99;
        original.push_back(s);
        original.push_back(nullptr);
        auto s2 = std::make_shared<tst_sub_struct>();
        s2->my_int = 88;
        original.push_back(s2);

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::vector<std::shared_ptr<tst_sub_struct>>>(yaml);

        REQUIRE(result->size() == 3);
        REQUIRE((*result)[0] != nullptr);
        REQUIRE((*result)[0]->my_int == 99);
        REQUIRE((*result)[1] == nullptr);
        REQUIRE((*result)[2] != nullptr);
        REQUIRE((*result)[2]->my_int == 88);
    }
}
