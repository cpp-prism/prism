#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_map2 (map<string,shared_ptr<tst_sub_struct>>) round trip", "[yaml][flow][map][shared_ptr][nullable]")
{
    SECTION("my_map2 one non-null entry flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 42;
        sub->my_bool = true;
        sub->my_string = "map2_flow";
        obj.my_map2["active"] = sub;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_map2.size() == 1);
        REQUIRE(result->my_map2["active"] != nullptr);
        REQUIRE(result->my_map2["active"]->my_int == 42);
        REQUIRE(result->my_map2["active"]->my_bool == true);
        REQUIRE(result->my_map2["active"]->my_string == "map2_flow");
    }

    SECTION("my_map2 null entry flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();
        obj.my_map2["absent"] = nullptr;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_map2.size() == 1);
        REQUIRE(result->my_map2["absent"] == nullptr);
    }

    SECTION("my_map2 mixed non-null and null entries flow round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 100;
        sub->my_string = "present";
        obj.my_map2["present"] = sub;
        obj.my_map2["null_entry"] = nullptr;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_map2.size() == 2);
        REQUIRE(result->my_map2["present"] != nullptr);
        REQUIRE(result->my_map2["present"]->my_int == 100);
        REQUIRE(result->my_map2["present"]->my_string == "present");
        REQUIRE(result->my_map2["null_entry"] == nullptr);
    }

    SECTION("my_map2 single non-null entry flow round trip")
    {
        tst_struct obj;
        obj.my_int = 4;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 99;
        sub->my_string = "only_one";
        obj.my_map2["only"] = sub;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 4);
        REQUIRE(result->my_map2.count("only") == 1);
        REQUIRE(result->my_map2["only"] != nullptr);
        REQUIRE(result->my_map2["only"]->my_int == 99);
        REQUIRE(result->my_map2["only"]->my_string == "only_one");
    }
}
