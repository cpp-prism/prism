#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - tst_struct all scalar fields set simultaneously round trip", "[json][tst_struct][scalar][all]")
{
    SECTION("all scalar fields positive values round trip")
    {
        tst_struct obj;
        obj.my_int = 42;
        obj.my_bool = true;
        obj.my_float = 1.5f;
        obj.my_double = 2.718281828;
        obj.my_longlong = 9876543210LL;
        obj.my_string = "test_all_scalars";
        obj.my_opt_str = "opt_all";
        obj.my_opt_int = 77;
        obj.my_sptr_int = std::make_shared<int>(55);
        obj.lang = language::english;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 42);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_float == Catch::Approx(1.5f).epsilon(1e-4));
        REQUIRE(result->my_double == Catch::Approx(2.718281828).epsilon(1e-9));
        REQUIRE(result->my_longlong == 9876543210LL);
        REQUIRE(result->my_string == "test_all_scalars");
        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(result->my_opt_str.value() == "opt_all");
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(result->my_opt_int.value() == 77);
        REQUIRE(result->my_sptr_int != nullptr);
        REQUIRE(*result->my_sptr_int == 55);
        REQUIRE(result->lang == language::english);
    }

    SECTION("all scalar fields negative/null values round trip")
    {
        tst_struct obj;
        obj.my_int = -99;
        obj.my_bool = false;
        obj.my_float = -0.5f;
        obj.my_double = -1.41421356;
        obj.my_longlong = -1234567890LL;
        obj.my_string = "";
        obj.my_opt_str = std::nullopt;
        obj.my_opt_int = std::nullopt;
        obj.my_sptr_int.reset();
        obj.lang = language::unknow;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == -99);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_float == Catch::Approx(-0.5f).epsilon(1e-4));
        REQUIRE(result->my_double == Catch::Approx(-1.41421356).epsilon(1e-9));
        REQUIRE(result->my_longlong == -1234567890LL);
        REQUIRE(result->my_string.empty());
        REQUIRE(!result->my_opt_str.has_value());
        REQUIRE(!result->my_opt_int.has_value());
        REQUIRE(result->my_sptr_int == nullptr);
        REQUIRE(result->lang == language::unknow);
    }
}
