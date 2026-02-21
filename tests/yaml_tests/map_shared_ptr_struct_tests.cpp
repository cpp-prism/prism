#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <map>

TEST_CASE("prismYaml - map<string, shared_ptr<struct>> round trip", "[yaml][map][smart-pointer][struct]")
{
    SECTION("map<string, shared_ptr<tst_sub_struct>> with non-null values serialization")
    {
        std::map<std::string, std::shared_ptr<tst_sub_struct>> m;
        auto s1 = std::make_shared<tst_sub_struct>();
        s1->my_int = 11;
        s1->my_string = "hello";
        m["item1"] = s1;

        std::string yaml = prism::yaml::toYamlStringFlow(m);
        REQUIRE(yaml.find("item1") != std::string::npos);
        REQUIRE(yaml.find("11") != std::string::npos);
        REQUIRE(yaml.find("hello") != std::string::npos);
    }

    SECTION("map<string, shared_ptr<tst_sub_struct>> with null value serialization")
    {
        std::map<std::string, std::shared_ptr<tst_sub_struct>> m;
        auto s = std::make_shared<tst_sub_struct>();
        s->my_int = 55;
        m["valid"] = s;
        m["null_item"] = nullptr;

        std::string yaml = prism::yaml::toYamlStringFlow(m);
        REQUIRE(yaml.find("null") != std::string::npos);
        REQUIRE(yaml.find("55") != std::string::npos);
    }

    SECTION("map<string, shared_ptr<tst_sub_struct>> with null value round trip")
    {
        std::map<std::string, std::shared_ptr<tst_sub_struct>> original;
        auto s = std::make_shared<tst_sub_struct>();
        s->my_int = 77;
        s->my_bool = true;
        s->my_string = "test_val";
        original["valid"] = s;
        original["null_item"] = nullptr;

        std::string yaml = prism::yaml::toYamlStringFlow(original);
        auto result = prism::yaml::fromYamlString<std::map<std::string, std::shared_ptr<tst_sub_struct>>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)["valid"] != nullptr);
        REQUIRE((*result)["valid"]->my_int == 77);
        REQUIRE((*result)["valid"]->my_bool == true);
        REQUIRE((*result)["valid"]->my_string == "test_val");
        REQUIRE((*result)["null_item"] == nullptr);
    }
}
