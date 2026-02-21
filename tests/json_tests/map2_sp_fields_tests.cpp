#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - my_map2 shared_ptr fields with various sub_struct values round trip", "[json][map2][shared_ptr]")
{
    SECTION("my_map2 entry with float and bool fields round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 10;
        sub->my_float = 3.14f;
        obj.my_map2["pi"] = sub;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map2.size() == 1);
        REQUIRE(result->my_map2.at("pi") != nullptr);
        REQUIRE(result->my_map2.at("pi")->my_int == 10);
        REQUIRE(result->my_map2.at("pi")->my_float == Catch::Approx(3.14f).epsilon(1e-4));
    }

    SECTION("my_map2 two entries one null one non-null round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 99;
        sub->my_longlong = 12345LL;
        obj.my_map2["valid"] = sub;
        obj.my_map2["empty"] = nullptr;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map2.size() == 2);
        REQUIRE(result->my_map2.at("valid") != nullptr);
        REQUIRE(result->my_map2.at("valid")->my_int == 99);
        REQUIRE(result->my_map2.at("valid")->my_longlong == 12345LL);
        REQUIRE(result->my_map2.at("empty") == nullptr);
    }

    SECTION("my_map2 entry with opt fields populated round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map2.clear();

        auto sub = std::make_shared<tst_sub_struct>();
        sub->my_int = 5;
        sub->my_opt_int = 42;
        obj.my_map2["opt"] = sub;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map2.at("opt") != nullptr);
        REQUIRE(result->my_map2.at("opt")->my_int == 5);
        REQUIRE(result->my_map2.at("opt")->my_opt_int.has_value());
        REQUIRE(result->my_map2.at("opt")->my_opt_int.value() == 42);
    }
}
