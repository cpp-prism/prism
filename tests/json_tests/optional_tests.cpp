#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("prismJson - optional<int> serialization", "[json][optional]")
{
    SECTION("serialize optional<int> with value")
    {
        std::optional<int> val = 42;
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "42");
    }

    SECTION("serialize optional<int> empty")
    {
        std::optional<int> val = std::nullopt;
        std::string json = prism::json::toJsonString(val);
        REQUIRE(json == "null");
    }

    SECTION("deserialize optional<int> from json")
    {
        SECTION("with value")
        {
            std::string json = "123";
            auto result = prism::json::fromJsonString<std::optional<int>>(json);
            REQUIRE(result->has_value());
            REQUIRE(**result == 123);
        }

        SECTION("null value")
        {
            std::string json = "null";
            auto result = prism::json::fromJsonString<std::optional<int>>(json);
            REQUIRE(!result->has_value());
        }
    }

    SECTION("round trip optional<int>")
    {
        std::optional<int> original = 999;
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::optional<int>>(json);
        REQUIRE(result->has_value());
        REQUIRE(**result == original.value());
    }

    SECTION("round trip optional<string>")
    {
        std::optional<std::string> original = "test optional string";
        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<std::optional<std::string>>(json);
        REQUIRE(result->has_value());
        REQUIRE(**result == original.value());
    }

    SECTION("optional in struct")
    {
        tst_sub_struct obj;
        obj.my_opt_int = 777;
        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_sub_struct>(json);
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == 777);
    }

    SECTION("nested optional")
    {
        std::optional<std::vector<std::optional<int>>> nested = std::vector<std::optional<int>>{1, std::nullopt, 3};
        std::string json = prism::json::toJsonString(nested);
        auto result = prism::json::fromJsonString<std::optional<std::vector<std::optional<int>>>>(json);
        REQUIRE(result->has_value());
        auto& vec = result->value();
        REQUIRE(vec.size() == 3);
        REQUIRE(vec[0].has_value());
        REQUIRE(vec[0].value() == 1);
        REQUIRE(!vec[1].has_value());
        REQUIRE(vec[2].has_value());
        REQUIRE(vec[2].value() == 3);
    }
}
