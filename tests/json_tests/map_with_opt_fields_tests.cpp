#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_map entries with optional fields in child structs round trip", "[json][map][opt][combo]")
{
    SECTION("my_map child with opt_str set round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 5;
        child.my_opt_str = "optional_in_child";
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        obj.my_map.emplace("c1", std::move(child));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map.at("c1").my_opt_str.has_value());
        REQUIRE(*result->my_map.at("c1").my_opt_str == "optional_in_child");
    }

    SECTION("my_map two entries one with opt_int set one without round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct c1;
        c1.my_int = 10;
        c1.my_opt_int = 99;
        c1.my_list_int.clear();
        c1.my_list_std_string.clear();
        obj.my_map.emplace("with_opt", std::move(c1));

        tst_struct c2;
        c2.my_int = 20;
        c2.my_opt_int = std::nullopt;
        c2.my_list_int.clear();
        c2.my_list_std_string.clear();
        obj.my_map.emplace("without_opt", std::move(c2));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.at("with_opt").my_opt_int.has_value());
        REQUIRE(*result->my_map.at("with_opt").my_opt_int == 99);
        REQUIRE(!result->my_map.at("without_opt").my_opt_int.has_value());
    }

    SECTION("my_map child with bool and opt_str combined round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map.clear();

        tst_struct child;
        child.my_int = 7;
        child.my_bool = true;
        child.my_opt_str = "yes_opt";
        child.my_list_int.clear();
        child.my_list_std_string.clear();
        obj.my_map.emplace("combo", std::move(child));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.at("combo").my_bool == true);
        REQUIRE(*result->my_map.at("combo").my_opt_str == "yes_opt");
    }
}
