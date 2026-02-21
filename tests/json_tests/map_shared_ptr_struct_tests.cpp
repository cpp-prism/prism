#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <map>

TEST_CASE("prismJson - map<string, shared_ptr<struct>> round trip", "[json][map][smart-pointer][struct]")
{
    SECTION("map<string, shared_ptr<tst_sub_struct>> with non-null values serialization")
    {
        std::map<std::string, std::shared_ptr<tst_sub_struct>> m;
        auto s1 = std::make_shared<tst_sub_struct>();
        s1->my_int = 11;
        s1->my_string = "hello";
        m["item1"] = s1;
        auto s2 = std::make_shared<tst_sub_struct>();
        s2->my_int = 22;
        s2->my_string = "world";
        m["item2"] = s2;

        std::string json = prism::json::toJsonString(m);
        REQUIRE(json.find("item1") != std::string::npos);
        REQUIRE(json.find("item2") != std::string::npos);
        REQUIRE(json.find("11") != std::string::npos);
        REQUIRE(json.find("hello") != std::string::npos);
    }

    SECTION("map<string, shared_ptr<tst_sub_struct>> with null value serialization")
    {
        std::map<std::string, std::shared_ptr<tst_sub_struct>> m;
        auto s = std::make_shared<tst_sub_struct>();
        s->my_int = 55;
        m["valid"] = s;
        m["null_item"] = nullptr;

        std::string json = prism::json::toJsonString(m);
        REQUIRE(json.find("null") != std::string::npos);
        REQUIRE(json.find("55") != std::string::npos);
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

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::map<std::string, std::shared_ptr<tst_sub_struct>>>(json);

        REQUIRE(result->size() == 2);
        REQUIRE((*result)["valid"] != nullptr);
        REQUIRE((*result)["valid"]->my_int == 77);
        REQUIRE((*result)["valid"]->my_bool == true);
        REQUIRE((*result)["valid"]->my_string == "test_val");
        REQUIRE((*result)["null_item"] == nullptr);
    }

    SECTION("tst_struct my_map2 field (map<string, shared_ptr<tst_sub_struct>>) round trip")
    {
        // my_map2 is initialized with {{"key1", make_shared<tst_sub_struct>()}, {"key2", {}}}
        // key2 maps to nullptr (null shared_ptr)
        tst_struct obj;
        obj.my_map2["struct_key"] = std::make_shared<tst_sub_struct>();
        obj.my_map2["struct_key"]->my_int = 99;
        obj.my_map2["null_key"] = nullptr;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map2.count("struct_key") == 1);
        REQUIRE(result->my_map2["struct_key"] != nullptr);
        REQUIRE(result->my_map2["struct_key"]->my_int == 99);
        REQUIRE(result->my_map2.count("null_key") == 1);
        REQUIRE(result->my_map2["null_key"] == nullptr);
    }
}
