#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format set<string> with special characters", "[yaml][block][set][string][special]")
{
    SECTION("standalone set<string> with colon-containing elements block round trip")
    {
        std::set<std::string> obj;
        obj.insert("server: localhost");
        obj.insert("host: 127.0.0.1");

        std::string yaml = prism::yaml::toYamlStringBlock(obj, 2);
        auto result = prism::yaml::fromYamlString<std::set<std::string>>(yaml);

        REQUIRE(result->size() == 2);
        REQUIRE(result->count("server: localhost") == 1);
        REQUIRE(result->count("host: 127.0.0.1") == 1);
    }

    SECTION("tst_struct my_set_str with boolean-looking elements block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"true", "false", "yes"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_set_str.count("true") == 1);
        REQUIRE(result->my_set_str.count("false") == 1);
        REQUIRE(result->my_set_str.count("yes") == 1);
    }

    SECTION("tst_struct my_set_str single element block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_set_str = {"only_one"};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_set_str.size() == 1);
        REQUIRE(result->my_set_str.count("only_one") == 1);
    }
}
