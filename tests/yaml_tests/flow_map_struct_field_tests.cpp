#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_map as struct field round trip", "[yaml][flow][map][struct]")
{
    SECTION("my_map with two entries flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct s1;
        s1.my_int = 10;
        s1.my_bool = true;
        s1.my_string = "val_one";
        s1.my_list_int.clear();
        s1.my_list_std_string.clear();
        obj.my_map["one"] = std::move(s1);

        tst_struct s2;
        s2.my_int = 20;
        s2.my_bool = false;
        s2.my_string = "val_two";
        s2.my_list_int.clear();
        s2.my_list_std_string.clear();
        obj.my_map["two"] = std::move(s2);

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_map.size() == 2);
        REQUIRE(result->my_map["one"].my_int == 10);
        REQUIRE(result->my_map["one"].my_bool == true);
        REQUIRE(result->my_map["one"].my_string == "val_one");
        REQUIRE(result->my_map["two"].my_int == 20);
        REQUIRE(result->my_map["two"].my_bool == false);
        REQUIRE(result->my_map["two"].my_string == "val_two");
    }

    SECTION("my_map empty flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        // my_map empty by default

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_map.empty());
    }

    SECTION("my_map single entry with numeric fields flow round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct s;
        s.my_int = 42;
        s.my_bool = true;
        s.my_longlong = 1234567890LL;
        s.my_list_int.clear();
        s.my_list_std_string.clear();
        obj.my_map["entry"] = std::move(s);

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map["entry"].my_int == 42);
        REQUIRE(result->my_map["entry"].my_bool == true);
        REQUIRE(result->my_map["entry"].my_longlong == 1234567890LL);
    }
}
