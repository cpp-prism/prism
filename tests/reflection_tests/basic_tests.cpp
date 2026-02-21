#include "../models/test_model.h"
#include "../../include/prism/prism.hpp"
#include <catch2/catch_test_macros.hpp>
#include <algorithm>

TEST_CASE("prism::reflection - field attributes", "[reflection]")
{
    SECTION("get field attribute")
    {
        auto attr = prism::attributes::getFieldAttr<tst_struct, prism::json::attributes::Attr_json_alias>("my_int");
        REQUIRE(attr.has_value());
        REQUIRE(std::string(attr.value()) == "MY_INT1");
    }

    SECTION("get class attribute")
    {
        auto attr = prism::attributes::getClassAttr<database_table, prism::sql::attributes::Attr_sql_class_alias>();
        REQUIRE(attr.has_value());
        REQUIRE(std::string(attr.value()) == "table_database_table");
    }
}

TEST_CASE("prism::reflection - for_each_fields", "[reflection]")
{
    SECTION("iterate all fields")
    {
        tst_struct val;
        int count = 0;
        std::vector<std::string> names;
        prism::reflection::for_each_fields(val, [&](const char* fname, auto&&) {
            count++;
            names.push_back(fname);
        });
        // tst_struct has 30 fields registered in PRISM_FIELDS (24 original + 6 new)
        REQUIRE(count == 30);
        REQUIRE(std::find(names.begin(), names.end(), "my_int") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "my_string") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "lang") != names.end());
    }

    SECTION("iterate base class fields")
    {
        Derived d;
        int count = 0;
        std::vector<std::string> names;
        prism::reflection::for_each_fields(d, [&](const char* fname, auto&&) {
            count++;
            names.push_back(fname);
        });
        // Derived has: b1_int, b1_float, b2_int, b2_float, d_int, d_float = 6 fields
        REQUIRE(count == 6);
        REQUIRE(std::find(names.begin(), names.end(), "b1_int") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "b2_int") != names.end());
        REQUIRE(std::find(names.begin(), names.end(), "d_int") != names.end());
    }
}

TEST_CASE("prism::reflection - field_do", "[reflection]")
{
    SECTION("access existing field")
    {
        tst_struct val;
        val.my_int = 42;
        prism::reflection::field_do(val, "my_int", [&](auto&& v) {
            if constexpr (std::is_same_v<std::decay_t<decltype(v)>, int>) {
                REQUIRE(v == 42);
            }
        });
    }

    SECTION("access non-existing field")
    {
        tst_struct val;
        bool found = prism::reflection::field_do(val, "non_existing_field", [&](auto&&) {
            REQUIRE(false);
        });
        REQUIRE(found == false);
    }

    SECTION("access base class field")
    {
        Derived d;
        d.b1_int = 100;
        prism::reflection::field_do(d, "b1_int", [&](auto&& v) {
            if constexpr (std::is_same_v<std::decay_t<decltype(v)>, int>) {
                REQUIRE(v == 100);
            }
        });
    }

    SECTION("modify field via field_do")
    {
        tst_struct val;
        val.my_int = 42;
        prism::reflection::field_do(val, "my_int", [&](auto&& v) {
            if constexpr (std::is_same_v<std::decay_t<decltype(v)>, int>) {
                v = 99;
            }
        });
        REQUIRE(val.my_int == 99);
    }
}
