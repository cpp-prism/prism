#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_list_sp elements with scalar fields only round trip", "[yaml][block][list_sp][scalar]")
{
    SECTION("my_list_sp single element with int and bool block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct entry;
        entry.my_int = 42;
        entry.my_bool = true;
        entry.my_list_int.clear();
        entry.my_list_std_string.clear();
        obj.my_list_sp.push_back(std::move(entry));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_sp.size() == 1);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_int == 42);
        REQUIRE(it->my_bool == true);
    }

    SECTION("my_list_sp two elements with distinct int values block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 10; i <= 20; i += 10) {
            tst_struct entry;
            entry.my_int = i;
            entry.my_list_int.clear();
            entry.my_list_std_string.clear();
            obj.my_list_sp.push_back(std::move(entry));
        }

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_sp.size() == 2);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_int == 10);
        ++it;
        REQUIRE(it->my_int == 20);
    }

    SECTION("my_list_sp element with string field block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct entry;
        entry.my_int = 7;
        entry.my_string = "yaml_str";
        entry.my_list_int.clear();
        entry.my_list_std_string.clear();
        obj.my_list_sp.push_back(std::move(entry));

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_list_sp.size() == 1);
        REQUIRE(result->my_list_sp.front().my_string == "yaml_str");
    }
}
